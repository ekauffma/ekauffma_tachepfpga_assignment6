# ekauffma_tachepfpga_assignment6

Without any pragmas, the resource utilization is as follows:

`example_0.cpp`

    +-----------------------------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |                    Modules                    | Issue|       | Latency |  Latency  | Iteration|         | Trip |          |      |         |           |           |     |
    |                    & Loops                    | Type | Slack | (cycles)|    (ns)   |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF    |    LUT    | URAM|
    +-----------------------------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |+ example                                      |     -|   9.23|       99|  2.475e+03|         -|      100|     -|        no|     -|  3 (~0%)|  662 (~0%)|  687 (~0%)|    -|
    | + grp_example_Pipeline_VITIS_LOOP_25_1_fu_38  |     -|  16.80|       32|    800.000|         -|       32|     -|        no|     -|        -|    7 (~0%)|   48 (~0%)|    -|
    |  o VITIS_LOOP_25_1                            |     -|  18.25|       30|    750.000|         1|        1|    30|       yes|     -|        -|          -|          -|    -|
    | + grp_example_Pipeline_VITIS_LOOP_5_1_fu_44   |     -|  16.79|       32|    800.000|         -|       32|     -|        no|     -|        -|   13 (~0%)|   59 (~0%)|    -|
    |  o VITIS_LOOP_5_1                             |     -|  18.25|       30|    750.000|         2|        1|    30|       yes|     -|        -|          -|          -|    -|
    | + grp_example_Pipeline_VITIS_LOOP_11_1_fu_52  |     -|   9.23|       30|    750.000|         -|       30|     -|        no|     -|  3 (~0%)|   13 (~0%)|  329 (~0%)|    -|
    |  o VITIS_LOOP_11_1                            |     -|  18.25|       28|    700.000|         2|        1|    28|       yes|     -|        -|          -|          -|    -|
    | + grp_example_Pipeline_VITIS_LOOP_17_1_fu_58  |     -|  16.79|       32|    800.000|         -|       32|     -|        no|     -|        -|   13 (~0%)|   59 (~0%)|    -|
    |  o VITIS_LOOP_17_1                            |     -|  18.25|       30|    750.000|         2|        1|    30|       yes|     -|        -|          -|          -|    -|
    +-----------------------------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+-----------+-----------+-----+

Next I tried unrolling all the loops:

`example_1.cpp`

    +---------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |          Modules          | Issue|       | Latency |  Latency  | Iteration|         | Trip |          |      |         |            |            |     |
    |          & Loops          | Type | Slack | (cycles)|    (ns)   |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF     |     LUT    | URAM|
    +---------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |+ example                  |     -|   9.26|       63|  1.575e+03|         -|       64|     -|        no|     -|  84 (1%)|  3154 (~0%)|  7774 (~0%)|    -|
    | + grp_compute_blur_fu_82  |     -|   9.99|       15|    375.000|         -|       15|     -|        no|     -|  84 (1%)|  2160 (~0%)|  7318 (~0%)|    -|
    | + grp_write_data_fu_87    |     -|  17.52|       14|    350.000|         -|       14|     -|        no|     -|        -|    15 (~0%)|   325 (~0%)|    -|
    | o VITIS_LOOP_5_1          |     -|  18.25|       30|    750.000|         2|        1|    30|       yes|     -|        -|           -|           -|    -|
    +---------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+------------+------------+-----+

The latency went down, but the resource utilization went way up. This makes sense since unrolling the loop will increase parallelization, which will end up being more resource-heavy.

Next I tried adding `#pragma HLS ALLOCATION`, to try to combat the increase in resource utilization.
I see that there are two additions and one division in `complete_blur`, so I tried limiting the number of adders and dividers to 1.

`example_2.cpp`

    +---------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |          Modules          | Issue|       | Latency |  Latency  | Iteration|         | Trip |          |      |         |            |            |     |
    |          & Loops          | Type | Slack | (cycles)|    (ns)   |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF     |     LUT    | URAM|
    +---------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |+ example                  |     -|   9.26|       63|  1.575e+03|         -|       64|     -|        no|     -|  84 (1%)|  3154 (~0%)|  7774 (~0%)|    -|
    | + grp_compute_blur_fu_82  |     -|   9.99|       15|    375.000|         -|       15|     -|        no|     -|  84 (1%)|  2160 (~0%)|  7318 (~0%)|    -|
    | + grp_write_data_fu_87    |     -|  17.52|       14|    350.000|         -|       14|     -|        no|     -|        -|    15 (~0%)|   325 (~0%)|    -|
    | o VITIS_LOOP_5_1          |     -|  18.25|       30|    750.000|         2|        1|    30|       yes|     -|        -|           -|           -|    -|
    +---------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+------------+------------+-----+

As can be seen, this changed absolutely nothing.

I then replaced all instances of `UNROLL` with `#pragma HLS PIPELINE II=2` and got the following results:

`example_3.cpp`

    +---------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |          Modules          | Issue|       | Latency | Latency | Iteration|         | Trip |          |      |         |            |            |     |
    |          & Loops          | Type | Slack | (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF     |     LUT    | URAM|
    +---------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |+ example                  |     -|   9.96|       31|  775.000|         -|       32|     -|        no|     -|  84 (1%)|  3324 (~0%)|  7988 (~0%)|    -|
    | + grp_read_data_fu_42     |    II|  17.52|       15|  375.000|         -|       15|     -|       yes|     -|        -|   913 (~0%)|   215 (~0%)|    -|
    | + grp_compute_blur_fu_48  |     -|  10.69|        1|   25.000|         -|        2|     -|       yes|     -|  84 (1%)|   834 (~0%)|  7298 (~0%)|    -|
    | + grp_write_data_fu_98    |    II|  17.52|       14|  350.000|         -|       15|     -|       yes|     -|        -|   655 (~0%)|   325 (~0%)|    -|
    +---------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+

This lowers resource utilization for `complete_blur`, but does not affect the others. It seems like a good start though. Just for comparison I also looked at `#pragma HLS PIPELINE II=1`.
This brought the latency down slightly:

`example_4.cpp`

    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |             Modules             | Issue|       | Latency | Latency | Iteration|         | Trip |          |      |         |            |            |     |
    |             & Loops             | Type | Slack | (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF     |     LUT    | URAM|
    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |+ example                        |     -|   9.26|       29|  725.000|         -|       30|     -|        no|     -|  84 (1%)|  1823 (~0%)|  7990 (~0%)|    -|
    | + grp_read_data_fu_26           |    II|  17.52|       15|  375.000|         -|       15|     -|       yes|     -|        -|   913 (~0%)|   215 (~0%)|    -|
    | + call_ret1_compute_blur_fu_32  |     -|  10.72|        0|    0.000|         -|        1|     -|       yes|     -|  84 (1%)|           -|  7308 (~0%)|    -|
    | + grp_write_data_fu_66          |    II|  17.52|       14|  350.000|         -|       15|     -|       yes|     -|        -|   879 (~0%)|   325 (~0%)|    -|
    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+

The total number of flip flops is also lower.

Next I tried adding `#pragma HLS DATAFLOW`. In order to get it to run without error I had to remove the loop that initialized `temp2` to 0, but it still passed all tests.

`example_5.cpp`

    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |             Modules             | Issue|       | Latency | Latency | Iteration|         | Trip |          |      |         |            |            |     |
    |             & Loops             | Type | Slack | (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF     |     LUT    | URAM|
    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+
    |+ example*                       |     -|   9.23|       30|  750.000|         -|       15|     -|  dataflow|     -|  84 (1%)|  8559 (~0%)|  12969 (1%)|    -|
    | + grp_read_data_fu_28           |    II|  17.52|       15|  375.000|         -|       15|     -|       yes|     -|        -|  1874 (~0%)|   496 (~0%)|    -|
    | + call_ret1_compute_blur_fu_34  |     -|  10.69|        0|    0.000|         -|        1|     -|       yes|     -|  84 (1%)|   870 (~0%)|  7487 (~0%)|    -|
    | + grp_write_data_fu_68          |    II|  17.52|       13|  325.000|         -|       14|     -|       yes|     -|        -|    15 (~0%)|   336 (~0%)|    -|
    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+---------+------------+------------+-----+

This still did not change DSP usage.

Next I figured that `int` might be overkill for the number of bits required, so I tried to use `ap_int<24>`. Here is the result:

    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+----------+------------+------------+-----+
    |             Modules             | Issue|       | Latency | Latency | Iteration|         | Trip |          |      |          |            |            |     |
    |             & Loops             | Type | Slack | (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |    DSP   |     FF     |     LUT    | URAM|
    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+----------+------------+------------+-----+
    |+ example*                       |     -|   8.35|       31|  775.000|         -|       15|     -|  dataflow|     -|  56 (~0%)|  7900 (~0%)|  12511 (1%)|    -|
    | + grp_read_data_fu_30           |    II|  17.52|       15|  375.000|         -|       15|     -|       yes|     -|         -|  1410 (~0%)|   496 (~0%)|    -|
    | + call_ret1_compute_blur_fu_36  |     -|   9.81|        0|    0.000|         -|        1|     -|       yes|     -|  56 (~0%)|   674 (~0%)|  7011 (~0%)|    -|
    | + grp_write_data_fu_70          |    II|  17.52|       14|  350.000|         -|       15|     -|       yes|     -|         -|    16 (~0%)|   354 (~0%)|    -|
    +---------------------------------+------+-------+---------+---------+----------+---------+------+----------+------+----------+------------+------------+-----+

This definitely lowered the DSP usage, so I tried to decrease again to `ap_int<10>`, since that is the minimum number of bits required for a signed integer that is max 500.

`example_6.cpp`

    +---------------------------+------+-------+---------+---------+----------+---------+------+----------+------+----------+------------+------------+-----+
    |          Modules          | Issue|       | Latency | Latency | Iteration|         | Trip |          |      |          |            |            |     |
    |          & Loops          | Type | Slack | (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |    DSP   |     FF     |     LUT    | URAM|
    +---------------------------+------+-------+---------+---------+----------+---------+------+----------+------+----------+------------+------------+-----+
    |+ example*                 |     -|  14.38|       34|  850.000|         -|       15|     -|  dataflow|     -|  28 (~0%)|  8519 (~0%)|  9611 (~0%)|    -|
    | + grp_read_data_fu_30     |    II|  17.52|       15|  375.000|         -|       15|     -|       yes|     -|         -|   598 (~0%)|   496 (~0%)|    -|
    | + grp_compute_blur_fu_36  |     -|  15.65|        3|   75.000|         -|        1|     -|       yes|     -|  28 (~0%)|  2105 (~0%)|  4129 (~0%)|    -|
    | + grp_write_data_fu_70    |    II|  17.52|       14|  350.000|         -|       15|     -|       yes|     -|         -|    16 (~0%)|   336 (~0%)|    -|
    +---------------------------+------+-------+---------+---------+----------+---------+------+----------+------+----------+------------+------------+-----+

I think this is the lowest possible DSP usage from changing to `ap_int`.

Next I tried changing the division to bit-shifting logic by approximating 1/3 as 21846/65536, so multiplying by 21846 and then bit shifting by 16.
This really didn't change much, probably due to assigning a DSP for the multiplication by a constant. 
I removed the `PIPELINE` pragmas, and acheived the following estimate:

`example.cpp`

    +-----------------------------------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |                       Modules                       | Issue|       | Latency |  Latency  | Iteration|         | Trip |          |      |         |           |           |     |
    |                       & Loops                       | Type | Slack | (cycles)|    (ns)   |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF    |    LUT    | URAM|
    +-----------------------------------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |+ example*                                           |     -|  13.94|      129|  3.225e+03|         -|       64|     -|  dataflow|     -|  1 (~0%)|  209 (~0%)|  393 (~0%)|    -|
    | + grp_read_data_fu_38                               |     -|  16.79|       32|    800.000|         -|       32|     -|        no|     -|        -|   13 (~0%)|   63 (~0%)|    -|
    |  o VITIS_LOOP_6_1                                   |     -|  18.25|       30|    750.000|         2|        1|    30|       yes|     -|        -|          -|          -|    -|
    | + grp_compute_blur_fu_46                            |     -|  13.94|       63|  1.575e+03|         -|       63|     -|        no|     -|  1 (~0%)|  103 (~0%)|  245 (~0%)|    -|
    |  + grp_compute_blur_Pipeline_VITIS_LOOP_18_1_fu_51  |     -|  13.94|       61|  1.525e+03|         -|       61|     -|        no|     -|  1 (~0%)|   98 (~0%)|  158 (~0%)|    -|
    |   o VITIS_LOOP_18_1                                 |    II|  18.25|       59|  1.475e+03|         6|        2|    28|       yes|     -|        -|          -|          -|    -|
    | + grp_write_data_fu_52                              |     -|  16.79|       32|    800.000|         -|       32|     -|        no|     -|        -|   13 (~0%)|   63 (~0%)|    -|
    |  o VITIS_LOOP_41_1                                  |     -|  18.25|       30|    750.000|         2|        1|    30|       yes|     -|        -|          -|          -|    -|
    +-----------------------------------------------------+------+-------+---------+-----------+----------+---------+------+----------+------+---------+-----------+-----------+-----+

The latency is higher than the initial code with no pragmas, but the DSP usage is down to 1, likely being used for the constant multiplier.
I'm going to call it here.
