/**
 * @file sys_mgr.c
 * System Manager module for Flight Software
 *
 * @author Dust. Na
 * Copyright 2014 ASCL Astris. All rights reserved.
 */

#include "include/sys_init_task.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <dev/cpu.h>

#include "../ant/isis_ants.h"
#include "../eps/clyde_eps.h"

#include "../sys/include/hsk_mgr.h"
#include "../sys/include/mode_mgr.h"
#include "../sys/include/packet_mgr.h"
#include "../sys/include/sys_common.h"
#include "../sys/include/sys_log.h"
#include "../sys/include/util_time.h"
#include "../sys/include/file_mgr.h"

#define MAX_ANT_HW_RESET_CNT		30		// Final version, Taeyoung, 180820
#define MAX_ANT_DEPLOY_CNT		5		// Final version, Taeyoung, 180904



/*
 * OBC CPU reset (System software reset)
 */
void system_sw_reset(void) {
	if (cpu_set_reset_cause)
		cpu_set_reset_cause(CPU_RESET_USER);	/////////////////////// Need to be modified, Taeyoung, 180820
	cpu_reset(); // OBC SW reset
}

/*
 * EPS Power reset (System hardware reset)
 */
void system_hw_reset(void) {
	clyde_eps_set_pcm_reset(CLYDE_EPS_ADDR, 0x0F); // All power bus reset
}


/*
 * Solar panel deployment
 */
void sys_mgr_solarpanel_deploy_select_batV(uint8_t sw_num) {
	uint8_t dep_trial_cnt = 0;

	sys_log_info("BCR::Solar Panels are Deploying ....");

	while (dep_trial_cnt < MAX_ANT_DEPLOY_CNT) {

		/* Deployment starting */
		clyde_eps_set_pdm_N_switch_on(CLYDE_EPS_ADDR, sw_num); // BatV(SW3 & SW4) ON

		vTaskDelay(30*100); // 30 sec delay

		clyde_eps_set_pdm_N_switch_off(CLYDE_EPS_ADDR, sw_num);

		dep_trial_cnt++;

	}
	// Solar panel deployment will be judged by Ground based on BCR4&5 voltage output, Taeyoung, 180718
}

/*
 * Deployment of VHF/UHF antenna subsystem
 */
void sys_mgr_antenna_deployemnt_trial(uint16_t ant_addr) {
	uint8_t dep_trial_cnt = 0;
	isis_ants_status_t ant_status;

	sys_log_info("ANT::Antenna deployment is currently tried ....");

	if (!isis_ants_status(ant_addr, &ant_status))	{  // Antenna initial i2c communication check, Taeyoung, 180814
		printf("isis antenna i2c communication error - get status \r\n");
		return;
	}

	while (dep_trial_cnt < MAX_ANT_DEPLOY_CNT) {
		isis_ants_arm(ant_addr);
		isis_ants_deploy_auto(ant_addr, 10); // 10 sec timeout for each antenna deployment, Auto deployment

		vTaskDelay(30*100);	// 30 sec delay

		isis_ants_status(ant_addr, &ant_status);
		if (!ant_status.ant[1].not_deployed && !ant_status.ant[3].not_deployed) // If both antennas are deployed, deployment is success! break
			break;

		dep_trial_cnt++;
		printf("dep_trial_cnt = [%d] \r\n", dep_trial_cnt);

	}
	isis_ants_disarm(ant_addr); // Disarm the antenna system before HW reset, Taeyoung, 180812
}
/*
 * Deployment of deployable subsystem
 * ANT1, ANT2, Solar Panel Deployment
 */
static unsigned int obc_boot_cnt;
void sys_mgr_subsystem_deploy(void) {

	uint16_t ant1_addr_A = ISIS_ANTS_DFL_ADDR_A;
	uint16_t ant1_addr_B = ISIS_ANTS_DFL_ADDR_B;
	uint16_t ant2_addr_A = ISIS_ANTS_VR_ADDR_A;
	uint16_t ant2_addr_B = ISIS_ANTS_VR_ADDR_B;
	isis_ants_status_t ant_status_A;
	isis_ants_status_t ant_status_B;


	// If deployable systems has not been deployed (1st deployment trial) - Wait 30 minutes
	if (obc_boot_cnt <= 3) {
		unsigned int unixtime_init = config_global_variable_unix_time_init_get();
		if(unixtime_init == 0)
			unixtime_init = 1542240000;

		set_cur_time(unixtime_init);
		printf("First boot delay started! \r\n");
		clyde_eps_set_watchdog_period(CLYDE_EPS_ADDR, 90);
		vTaskDelay(30*60*100);	// 30 min delay
	}

	/* Antenna 1 (Main) Deployment */
	sys_mgr_antenna_deployemnt_trial(ant1_addr_A);		// 5 times trials (10 sec * 5)
	sys_mgr_antenna_deployemnt_trial(ant1_addr_B);		// 5 times trials (10 sec * 5)

	isis_ants_status(ant1_addr_A, &ant_status_A);
	isis_ants_status(ant1_addr_B, &ant_status_B);
	if (ant_status_A.ant[1].not_deployed || ant_status_A.ant[3].not_deployed || ant_status_B.ant[1].not_deployed || ant_status_B.ant[3].not_deployed) {
		sys_log_info("ANT::Main antenna deployment is failed!!");
		if (obc_boot_cnt < MAX_ANT_HW_RESET_CNT){
			system_hw_reset(); // if i2c communication is not possible temporarily? or indication switch returns not reliable state, Taeyoung, 180820
		}
	}

	/* Antenna 2 (VR) Deployment */
	sys_mgr_antenna_deployemnt_trial(ant2_addr_A);		// 5 times trials (10 sec * 5)
	sys_mgr_antenna_deployemnt_trial(ant2_addr_B);		// 5 times trials (10 sec * 5)

	/* Solar Panel Deployment */
	sys_mgr_solarpanel_deploy_select_batV(CLYDE_EPS_PDM_SW3_BAT_V);		// Taeyoung, 180904
	sys_mgr_solarpanel_deploy_select_batV(CLYDE_EPS_PDM_SW4_BAT_V);

	clyde_eps_set_watchdog_period(CLYDE_EPS_ADDR, 4);
}

void sys_global_variable_first_update(void){
	// ----------------------------------------------------------------------------------------------------------------------------
	// Log Relative
	// ----------------------------------------------------------------------------------------------------------------------------
	log_global_variable_sid01_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid02_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid03_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid04_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid08_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid0B_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid14_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid15_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid16_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid17_store_set(0);
	vTaskDelay(1);
	log_global_variable_sid18_store_set(0);
	vTaskDelay(1);
	log_global_variable_sidEE_store_set(1);	// Always on
	vTaskDelay(1);

	 //----------------------------------------------------------------------------------------------------------------------------
	 // ADCS Commissioning Relative
	 // ----------------------------------------------------------------------------------------------------------------------------
	adcs_global_variable_commissioning_step_1_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_2_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_3_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_4_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_5_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_6_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_7_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_8_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_9_1_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_9_2_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_10_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_11_set(0);
	vTaskDelay(1);
	adcs_global_variable_commissioning_step_12_set(0);
	vTaskDelay(1);

//
//	 //----------------------------------------------------------------------------------------------------------------------------
//	 // Configuration Relative
//	 // ----------------------------------------------------------------------------------------------------------------------------
	config_global_variable_obc_boot_cause_set(0);	// Set to 0
	vTaskDelay(1);
	config_global_variable_unix_time_init_set(1542240000);	// Set to 1542240000
	vTaskDelay(1);
	config_global_variable_power_safe_in_set(4);	// Set to 4
	vTaskDelay(1);
	config_global_variable_power_safe_out_set(8);	// Set to 8
	vTaskDelay(1);
	config_global_variable_p_beacon_sec_set(60);	// Set to 60
	vTaskDelay(1);
	config_global_variable_beacon_hsk_ratio_set(5);	// Set to 5
	vTaskDelay(1);
	config_global_variable_err_cnt_thres_set(30);	// Set to 30
	vTaskDelay(1);
	config_global_variable_p_beacon_tx_sec_set(45);	// Set to 45
	vTaskDelay(1);
	config_global_variable_uhf_bitrate_set(1);	// Set to 1200 bps(=1)
	vTaskDelay(1);
	config_global_variable_stx_bitrate_set(3);	// Set to initial mode(=3)
	vTaskDelay(1);
	config_global_variable_stx_freq_dft_set(8);		// Set to freq default(=8)
	vTaskDelay(1);
	config_global_variable_ack_hsk_num_set(1);	// Set to 1
	vTaskDelay(1);
	config_global_variable_p_gnd_rx_sec_set(1);	// Set to 1
	vTaskDelay(1);
	config_global_variable_p_sysmode_sec_set(65);	// Set to 65
	vTaskDelay(1);
	config_global_variable_stx_max_cnt_set(40);	// Set to 40
	vTaskDelay(1);
	config_global_variable_detumb_rate_set(4);	// Set to 4
	vTaskDelay(1);
	config_global_variable_p_detumb_sec_set(60);	// Set to 60
	vTaskDelay(1);
	config_global_variable_cam_format_set(2);	// Set to 2
	vTaskDelay(1);
	config_global_variable_cam_exposure_ms_set(100);	// Set to 100
	vTaskDelay(1);

	// ----------------------------------------------------------------------------------------------------------------------------
	// Mode Relative
	// ----------------------------------------------------------------------------------------------------------------------------
	mode_global_variable_ps_beac_on_set(1);		// Set to 1
	vTaskDelay(1);
	mode_global_variable_ps_sun_point_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_ps_est_mode_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_stx_tx_mode_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_trx_tx_mode_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_psmode_cur_set(1);	// Set to 1
	vTaskDelay(1);
	mode_global_variable_gps_enable_set(1);	// Set to 1
	vTaskDelay(1);
	mode_global_variable_magmt_mode_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_cam_if_com_set(0);	// Set to 0
	vTaskDelay(1);

	//
	mode_global_variable_adcs_op_enable_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_rate_gyro_dft_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_detumb_mode_set(0);	// Set to 0
	vTaskDelay(1);
	mode_global_variable_est_mode_set(2);	// Set to 2
	vTaskDelay(1);

}
/*
 * System Initializing
 * Initial Mode
 */
void vTaskSysInit(void * pvParameters) {


	////////////////////////////// K2SAT operation concept, Taeyoung Kim, last modified(18.09.24)


	// ------------------------------------------------------------------------------------------------------------------------------
	vTaskDelay(2*100);
	sys_log_info("SYS::System mode set to INITIAL MODE (18/10/01). \r\n");
	obc_boot_cnt = config_global_variable_obc_boot_cnt_get();
	if (obc_boot_cnt<3){
		sys_global_variable_first_update();		// only for first time
	}
	printf("obc_boot_cnt = [%d] \r\n", obc_boot_cnt);
//
	uint8_t uhf_bitrate = (uint8_t) config_global_variable_uhf_bitrate_get();		// 1
	uint8_t stx_bitrate =  (uint8_t) config_global_variable_stx_bitrate_get();	// 3
	uint8_t stx_freq =  (uint8_t) config_global_variable_stx_freq_dft_get();		// 8

	if((stx_bitrate == 0)&&(stx_freq == 0)){
			uhf_bitrate = 1;
			stx_bitrate = 3;
			stx_freq = 8;
	}

	// ------------------------------------------------------------------------------------------------------------------------------
	vTaskDelay(10*100);

	clyde_stx_reset_register(CLYDE_STX_ADDR);
	isis_tx_set_bitrate(ISIS_TX_ADDR, uhf_bitrate);
	clyde_stx_data_rate_control(CLYDE_STX_ADDR, stx_bitrate);
	clyde_stx_frequency_offset_control(CLYDE_STX_ADDR, stx_freq);

	// ------------------------------------------------------------------------------------------------------------------------------
	/* 1. UHF/VHF Main & VR antenna, and Solar panel deployment */
	if (obc_boot_cnt < MAX_ANT_HW_RESET_CNT){
		sys_mgr_subsystem_deploy();
	}

	// ------------------------------------------------------------------------------------------------------------------------------
	/* 2. Start System Monitoring, Collecting HSK data in every 5 mins and WOD in every 1 min */
	xTaskCreate(vTaskSysMonitor, (const signed char *) "SYS_MONITOR", 1024*4, NULL, 3, NULL);
	printf("Sys monitor has started \r\n");
	vTaskDelay(2*100);	// 2 sec delay
//
//	// ------------------------------------------------------------------------------------------------------------------------------
//	/* 3. Start Periodic Beacon Transmit in every 30 sec */
	xTaskCreate(vTaskBeaconTx, (const signed char*) "Beacon_TX", 1024*4, NULL, 3, &handle_beacon);
	printf("Beacon_TX has started \r\n");
	vTaskDelay(2*100);	// 2 sec delay
////
//	// ------------------------------------------------------------------------------------------------------------------------------
//	/* 4. Start Receive Ground Signal in every 100 ms */
	xTaskCreate(vTaskGroundRx, (const signed char*) "GND_RCV", 1024*4, NULL, 3, NULL);
	printf("vTaskGroundRx has started \r\n");
	vTaskDelay(2*100);	// 2 sec delay
//
////	// ------------------------------------------------------------------------------------------------------------------------------
////
////	/* 5. Start CubeSat Operation Mode Monitoring in every 10 sec */
	xTaskCreate(vTaskModeMgr, (const signed char*) "MODE_MGR", 1024*4, NULL, 3, NULL);
	printf("vTaskModeMgr has started \r\n");
	vTaskDelay(2*100);	// 2 sec delay
//
//	// ------------------------------------------------------------------------------------------------------------------------------
//
//	/* 6. Start CubeSat ADCS Detumbling in every 10 sec */
	xTaskCreate(vTaskADCSOps, (const signed char*) "ADCS detumbling", 1024*4, NULL, 3, &handle_adcs);
	printf("vTaskADCSOps has started \r\n");

	// ------------------------------------------------------------------------------------------------------------------------------
	/** End of Initial Mode **/
	vTaskDelete(NULL);

}
