#ifndef CHARSET_H
#define CHARSET_H

// This header is composed of macros to make working with TI's character set easier

#define xv_AsciiToTI(ch) \
  (ch == '[' ? '\xc1' : ch)

#define CH(ch) '\xch'

#define c_nVar "\x01"
#define c_uVar "\x02"
#define c_vVar "\x03"
#define c_wVar "\x04"
#define c_rTri "\x05"
#define c_boldArrUp "\x06"
#define c_boldArrDown "\x07"
#define c_integral "\x08"
#define c_wideX "\x09"      /// I don"t know what this represents
#define c_plotBox "\x0a"
#define c_plotPlus "\x0b"
#define c_plotSquare "\x0c"
#define c_transpose "\x0d"
#define c_super3 "\x0e"
#define c_boldF "\x0f"
#define c_root "\x10"
#define c_recipSuper "\x11"
#define c_squareSuper "\x12"
#define c_angleMeas "\x13"
#define c_degree "\x14"
#define c_radian "\x15"
#define c_transposeSuper "\x16"
#define c_lessEq "\x17"
#define c_notEq "\x18"
#define c_grEq "\x19"
#define c_negSuper "\x1a"
#define c_EE "\x1b"
#define c_sto "\x1c"
#define c_small10 "\x1d"
#define c_arrUp "\x1e"
#define c_arrDown "\x1f"

#define c_theta "\x5b"

#define c_cursEq "\x7f"
#define c_small0 "\x80"
#define c_small1 "\x81"
#define c_small2 "\x82"
#define c_small3 "\x83"
#define c_small4 "\x84"
#define c_small5 "\x85"
#define c_small6 "\x86"
#define c_small7 "\x87"
#define c_small8 "\x88"
#define c_small9 "\x89"
//Special characters, do later
#define c_forwTick "\xb6"
#define c_backTick "\xb7"
#define c_diuresis "\xb8"
#define c_upQuest "\xb9"
#define c_upExcl "\xba"
#define c_alpha "\xbb"
#define c_beta "\xbc"
#define c_gamma "\xbd"
#define c_deltaCap "\xbe"
#define c_delta "\xbf"
#define c_epsilon "\xc0"
#define c_forwwBracket "\xc1"
#define c_lambda "\xc2"
#define c_mu "\xc3"
#define c_pi "\xc4"
#define c_rho "\xc5"
#define c_sigmaCap "\xc6"
#define c_sigma "\xc7"
#define c_tau "\xc8"
#define c_phi "\xc9"
#define c_omega "\xca"
#define c_xBar "\xcb"
#define c_yBar "\xcc"
#define c_xSuper "\xcd"
#define c_ellipsis "\xce"
#define c_lTri "\xcf"

#define c_skinnyDiv "\xd1"
#define c_neg "\xd2"
#define c_2Super "\xd3"
#define c_plotCircle "\xd4"
#define c_3Super "\xd5"
#define c_colon "\xd6"
#define c_iVar "\xd7"
#define c_pCircum "\xd8"
#define c_chi "\xd9"
#define c_weirdF "\xda"
#define c_eVar "\xdb"
#define c_list "\xdc"
#define c_boldN "\xdd"
#define c_doubleCloseParen "\xde"
#define c_boldArrRight "\xdf"
#define c_curs "\xe0"
#define c_curs2 "\xe1"
#define c_cursA "\xe2"
#define c_cursLow "\xe3"
#define c_cursIns "\xe4"
#define c_cursIns2 "\xe5"
#define c_cursInsA "\xe6"
#define c_cursInsLow "\xe7"
#define c_graphNormal "\xe8"
#define c_graphBold "\xe9"
#define c_graphAbove "\xea"
#define c_graphBelow "\xeb"
#define c_graphCircleTrace "\xec"
#define c_graphCircle "\xed"
#define c_graphDot "\xee"
#define c_bigArrUp "\xef"
#define c_bigArrDown "\xf0"
#define c_cursCheck "\xf1"
#define c_4Super "\xf2"
#define c_eszett "\xf4"
#define c_space "\xf5"
#define c_frac "\xf6"
#define c_cursDotOut "\xf7"
#define c_graphDotBold "\xf8"
#define c_plotDot "\xf9"


#define i_nVar 0x01
#define i_uVar 0x02
#define i_vVar 0x03
#define i_wVar 0x04
#define i_rTri 0x05
#define i_boldArrUp 0x06
#define i_boldArrDown 0x07
#define i_integral 0x08
#define i_wideX 0x09      /// I don"t know what this represents
#define i_plotBox 0x0a
#define i_plotPlus 0x0b
#define i_plotSquare 0x0c
#define i_transpose 0x0d
#define i_super3 0x0e
#define i_boldF 0x0f
#define i_root 0x10
#define i_recipSuper 0x11
#define i_squareSuper 0x12
#define i_angleMeas 0x13
#define i_degree 0x14
#define i_radian 0x15
#define i_transposeSuper 0x16
#define i_lessEq 0x17
#define i_notEq 0x18
#define i_grEq 0x19
#define i_negSuper 0x1a
#define i_EE 0x1b
#define i_sto 0x1c
#define i_small10 0x1d
#define i_arrUp 0x1e
#define i_arrDown 0x1f

#define i_theta 0x5b

#define i_cursEq 0x7f
#define i_small0 0x80
#define i_small1 0x81
#define i_small2 0x82
#define i_small3 0x83
#define i_small4 0x84
#define i_small5 0x85
#define i_small6 0x86
#define i_small7 0x87
#define i_small8 0x88
#define i_small9 0x89
//Special characters, do later
#define i_forwTick 0xb6
#define i_backTick 0xb7
#define i_diuresis 0xb8
#define i_upQuest 0xb9
#define i_upExcl 0xba
#define i_alpha 0xbb
#define i_beta 0xbc
#define i_gamma 0xbd
#define i_deltaCap 0xbe
#define i_delta 0xbf
#define i_epsilon 0xc0
#define i_forwwBracket 0xc1
#define i_lambda 0xc2
#define i_mu 0xc3
#define i_pi 0xc4
#define i_rho 0xc5
#define i_sigmaCap 0xc6
#define i_sigma 0xc7
#define i_tau 0xc8
#define i_phi 0xc9
#define i_omega 0xca
#define i_xBar 0xcb
#define i_yBar 0xcc
#define i_xSuper 0xcd
#define i_ellipsis 0xce
#define i_lTri 0xcf

#define i_skinnyDiv 0xd1
#define i_neg 0xd2
#define i_2Super 0xd3
#define i_plotCircle 0xd4
#define i_3Super 0xd5
#define i_colon 0xd6
#define i_iVar 0xd7
#define i_pCircum 0xd8
#define i_chi 0xd9
#define i_weirdF 0xda
#define i_eVar 0xdb
#define i_list 0xdc
#define i_boldN 0xdd
#define i_doubleCloseParen 0xde
#define i_boldArrRight 0xdf
#define i_curs 0xe0
#define i_curs2 0xe1
#define i_cursA 0xe2
#define i_cursLow 0xe3
#define i_cursIns 0xe4
#define i_cursIns2 0xe5
#define i_cursInsA 0xe6
#define i_cursInsLow 0xe7
#define i_graphNormal 0xe8
#define i_graphBold 0xe9
#define i_graphAbove 0xea
#define i_graphBelow 0xeb
#define i_graphCircleTrace 0xec
#define i_graphCircle 0xed
#define i_graphDot 0xee
#define i_bigArrUp 0xef
#define i_bigArrDown 0xf0
#define i_cursCheck 0xf1
#define i_4Super 0xf2
#define i_eszett 0xf4
#define i_space 0xf5
#define i_frac 0xf6
#define i_cursDotOut 0xf7
#define i_graphDotBold 0xf8
#define i_plotDot 0xf9


#endif
