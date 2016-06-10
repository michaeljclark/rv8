//
//  riscv-decode-args.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_decode_args_h
#define riscv_decode_args_h

typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_rd;
typedef uimm_t<5, S<19,15, B<4,0>>>                                          riscv_arg_rs1;
typedef uimm_t<5, S<24,20, B<4,0>>>                                          riscv_arg_rs2;
typedef uimm_t<5, S<31,27, B<4,0>>>                                          riscv_arg_rs3;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_frd;
typedef uimm_t<5, S<19,15, B<4,0>>>                                          riscv_arg_frs1;
typedef uimm_t<5, S<24,20, B<4,0>>>                                          riscv_arg_frs2;
typedef uimm_t<5, S<31,27, B<4,0>>>                                          riscv_arg_frs3;
typedef uimm_t<1, S<26,26, B<0,0>>>                                          riscv_arg_aq;
typedef uimm_t<1, S<25,25, B<0,0>>>                                          riscv_arg_rl;
typedef uimm_t<4, S<27,24, B<3,0>>>                                          riscv_arg_pred;
typedef uimm_t<4, S<23,20, B<3,0>>>                                          riscv_arg_succ;
typedef uimm_t<3, S<14,12, B<2,0>>>                                          riscv_arg_rm;
typedef simm_t<32, S<31,12, B<31,12>>>                                       riscv_arg_imm20;
typedef simm_t<21, S<31,12, B<20>,B<10,1>,B<11>,B<19,12>>>                   riscv_arg_jimm20;
typedef simm_t<12, S<31,20, B<11,0>>>                                        riscv_arg_imm12;
typedef uimm_t<12, S<31,20, B<11,0>>>                                        riscv_arg_csr12;
typedef simm_t<12, S<31,25, B<11,5>>, S<11,7, B<4,0>>>                       riscv_arg_simm12;
typedef simm_t<13, S<31,25, B<12>,B<10,5>>, S<11,7, B<4,1>,B<11>>>           riscv_arg_sbimm12;
typedef simm_t<5, S<19,15, B<4,0>>>                                          riscv_arg_zimm;
typedef uimm_t<5, S<24,20, B<4,0>>>                                          riscv_arg_shamt5;
typedef uimm_t<6, S<25,20, B<5,0>>>                                          riscv_arg_shamt6;
typedef uimm_t<1, S<12,12, B<0,0>>>                                          riscv_arg_crd0;
typedef uimm_t<3, S<4,2, B<2,0>>>                                            riscv_arg_crdq;
typedef uimm_t<3, S<9,7, B<2,0>>>                                            riscv_arg_crs1q;
typedef uimm_t<3, S<9,7, B<2,0>>>                                            riscv_arg_crs1rdq;
typedef uimm_t<3, S<4,2, B<2,0>>>                                            riscv_arg_crs2q;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_crd;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_crs1;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_crs1rd;
typedef uimm_t<5, S<6,2, B<4,0>>>                                            riscv_arg_crs2;
typedef uimm_t<3, S<4,2, B<2,0>>>                                            riscv_arg_cfrdq;
typedef uimm_t<3, S<4,2, B<2,0>>>                                            riscv_arg_cfrs2q;
typedef uimm_t<5, S<6,2, B<4,0>>>                                            riscv_arg_cfrs2;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_cfrd;
typedef uimm_t<5, S<6,2, B<4,0>>>                                            riscv_arg_cimmsh5;
typedef uimm_t<6, S<12,12, B<5>>, S<6,2, B<4,0>>>                            riscv_arg_cimmsh6;
typedef simm_t<6, S<12,12, B<5>>, S<6,2, B<4,0>>>                            riscv_arg_cimmi;
typedef simm_t<18, S<12,12, B<17>>, S<6,2, B<16,12>>>                        riscv_arg_cimmui;
typedef simm_t<8, S<12,12, B<5>>, S<6,2, B<4,2>,B<7,6>>>                     riscv_arg_cimmlwsp;
typedef simm_t<9, S<12,12, B<5>>, S<6,2, B<4,3>,B<8,6>>>                     riscv_arg_cimmldsp;
typedef simm_t<10, S<12,12, B<9>>, S<6,2, B<4>,B<6>,B<8,7>,B<5>>>            riscv_arg_cimm16sp;
typedef simm_t<12, S<12,2, B<11>,B<4>,B<9,8>,B<10>,B<6>,B<7>,B<3,1>,B<5>>>   riscv_arg_cimmj;
typedef simm_t<9, S<12,10, B<8>,B<4,3>>, S<6,2, B<7,6>,B<2,1>,B<5>>>         riscv_arg_cimmb;
typedef simm_t<8, S<12,7, B<5,2>,B<7,6>>>                                    riscv_arg_cimmswsp;
typedef simm_t<9, S<12,7, B<5,3>,B<8,6>>>                                    riscv_arg_cimmsdsp;
typedef simm_t<10, S<12,7, B<5,4>,B<9,6>>>                                   riscv_arg_cimmsqsp;
typedef simm_t<10, S<12,5, B<5,4>,B<9,6>,B<2>,B<3>>>                         riscv_arg_cimm4spn;
typedef simm_t<7, S<12,10, B<5,3>>, S<6,5, B<2>,B<6>>>                       riscv_arg_cimmw;
typedef simm_t<8, S<12,10, B<5,3>>, S<6,5, B<7,6>>>                          riscv_arg_cimmd;
typedef simm_t<9, S<12,10, B<5,4>,B<8>>, S<6,5, B<7,6>>>                     riscv_arg_cimmq;

#endif
