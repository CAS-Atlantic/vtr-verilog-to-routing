// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Register Package auto-generated by `reggen` containing data structure

package edn_reg_pkg;

  // Param list
  parameter int NumEndPoints = 64;
  parameter int BootInsCmd = 1;
  parameter int BootGenCmd = 12291;

  ////////////////////////////
  // Typedefs for registers //
  ////////////////////////////
  typedef struct packed {
    struct packed {
      logic        q;
    } edn_cmd_req_done;
    struct packed {
      logic        q;
    } edn_fifo_err;
  } edn_reg2hw_intr_state_reg_t;

  typedef struct packed {
    struct packed {
      logic        q;
    } edn_cmd_req_done;
    struct packed {
      logic        q;
    } edn_fifo_err;
  } edn_reg2hw_intr_enable_reg_t;

  typedef struct packed {
    struct packed {
      logic        q;
      logic        qe;
    } edn_cmd_req_done;
    struct packed {
      logic        q;
      logic        qe;
    } edn_fifo_err;
  } edn_reg2hw_intr_test_reg_t;

  typedef struct packed {
    logic        q;
  } edn_reg2hw_regen_reg_t;

  typedef struct packed {
    struct packed {
      logic        q;
    } edn_enable;
    struct packed {
      logic        q;
    } cmd_fifo_rst;
    struct packed {
      logic        q;
    } auto_req_mode;
    struct packed {
      logic        q;
    } boot_req_dis;
  } edn_reg2hw_ctrl_reg_t;

  typedef struct packed {
    logic [31:0] q;
    logic        qe;
  } edn_reg2hw_sw_cmd_req_reg_t;

  typedef struct packed {
    logic [31:0] q;
    logic        qe;
  } edn_reg2hw_reseed_cmd_reg_t;

  typedef struct packed {
    logic [31:0] q;
    logic        qe;
  } edn_reg2hw_generate_cmd_reg_t;

  typedef struct packed {
    logic [31:0] q;
    logic        qe;
  } edn_reg2hw_max_num_reqs_between_reseeds_reg_t;


  typedef struct packed {
    struct packed {
      logic        d;
      logic        de;
    } edn_cmd_req_done;
    struct packed {
      logic        d;
      logic        de;
    } edn_fifo_err;
  } edn_hw2reg_intr_state_reg_t;

  typedef struct packed {
    struct packed {
      logic        d;
      logic        de;
    } req_mode_sm_sts;
    struct packed {
      logic        d;
      logic        de;
    } boot_inst_ack;
    struct packed {
      logic        d;
      logic        de;
    } internal_use;
  } edn_hw2reg_sum_sts_reg_t;

  typedef struct packed {
    struct packed {
      logic        d;
      logic        de;
    } cmd_rdy;
    struct packed {
      logic        d;
      logic        de;
    } cmd_sts;
  } edn_hw2reg_sw_cmd_sts_reg_t;

  typedef struct packed {
    struct packed {
      logic        d;
      logic        de;
    } sfifo_rescmd_err;
    struct packed {
      logic        d;
      logic        de;
    } sfifo_gencmd_err;
    struct packed {
      logic        d;
      logic        de;
    } fifo_write_err;
    struct packed {
      logic        d;
      logic        de;
    } fifo_read_err;
    struct packed {
      logic        d;
      logic        de;
    } fifo_state_err;
  } edn_hw2reg_err_code_reg_t;


  ///////////////////////////////////////
  // Register to internal design logic //
  ///////////////////////////////////////
  typedef struct packed {
    edn_reg2hw_intr_state_reg_t intr_state; // [144:143]
    edn_reg2hw_intr_enable_reg_t intr_enable; // [142:141]
    edn_reg2hw_intr_test_reg_t intr_test; // [140:137]
    edn_reg2hw_regen_reg_t regen; // [136:136]
    edn_reg2hw_ctrl_reg_t ctrl; // [135:132]
    edn_reg2hw_sw_cmd_req_reg_t sw_cmd_req; // [131:99]
    edn_reg2hw_reseed_cmd_reg_t reseed_cmd; // [98:66]
    edn_reg2hw_generate_cmd_reg_t generate_cmd; // [65:33]
    edn_reg2hw_max_num_reqs_between_reseeds_reg_t max_num_reqs_between_reseeds; // [32:0]
  } edn_reg2hw_t;

  ///////////////////////////////////////
  // Internal design logic to register //
  ///////////////////////////////////////
  typedef struct packed {
    edn_hw2reg_intr_state_reg_t intr_state; // [23:20]
    edn_hw2reg_sum_sts_reg_t sum_sts; // [19:14]
    edn_hw2reg_sw_cmd_sts_reg_t sw_cmd_sts; // [13:10]
    edn_hw2reg_err_code_reg_t err_code; // [9:0]
  } edn_hw2reg_t;

  // Register Address
  parameter logic [5:0] EDN_INTR_STATE_OFFSET = 6'h 0;
  parameter logic [5:0] EDN_INTR_ENABLE_OFFSET = 6'h 4;
  parameter logic [5:0] EDN_INTR_TEST_OFFSET = 6'h 8;
  parameter logic [5:0] EDN_REGEN_OFFSET = 6'h c;
  parameter logic [5:0] EDN_CTRL_OFFSET = 6'h 10;
  parameter logic [5:0] EDN_SUM_STS_OFFSET = 6'h 14;
  parameter logic [5:0] EDN_SW_CMD_REQ_OFFSET = 6'h 18;
  parameter logic [5:0] EDN_SW_CMD_STS_OFFSET = 6'h 1c;
  parameter logic [5:0] EDN_RESEED_CMD_OFFSET = 6'h 20;
  parameter logic [5:0] EDN_GENERATE_CMD_OFFSET = 6'h 24;
  parameter logic [5:0] EDN_MAX_NUM_REQS_BETWEEN_RESEEDS_OFFSET = 6'h 28;
  parameter logic [5:0] EDN_ERR_CODE_OFFSET = 6'h 2c;


  // Register Index
  typedef enum int {
    EDN_INTR_STATE,
    EDN_INTR_ENABLE,
    EDN_INTR_TEST,
    EDN_REGEN,
    EDN_CTRL,
    EDN_SUM_STS,
    EDN_SW_CMD_REQ,
    EDN_SW_CMD_STS,
    EDN_RESEED_CMD,
    EDN_GENERATE_CMD,
    EDN_MAX_NUM_REQS_BETWEEN_RESEEDS,
    EDN_ERR_CODE
  } edn_id_e;

  // Register width information to check illegal writes
  parameter logic [3:0] EDN_PERMIT [12] = '{
    4'b 0001, // index[ 0] EDN_INTR_STATE
    4'b 0001, // index[ 1] EDN_INTR_ENABLE
    4'b 0001, // index[ 2] EDN_INTR_TEST
    4'b 0001, // index[ 3] EDN_REGEN
    4'b 0001, // index[ 4] EDN_CTRL
    4'b 1111, // index[ 5] EDN_SUM_STS
    4'b 1111, // index[ 6] EDN_SW_CMD_REQ
    4'b 0001, // index[ 7] EDN_SW_CMD_STS
    4'b 1111, // index[ 8] EDN_RESEED_CMD
    4'b 1111, // index[ 9] EDN_GENERATE_CMD
    4'b 1111, // index[10] EDN_MAX_NUM_REQS_BETWEEN_RESEEDS
    4'b 1111  // index[11] EDN_ERR_CODE
  };
endpackage

