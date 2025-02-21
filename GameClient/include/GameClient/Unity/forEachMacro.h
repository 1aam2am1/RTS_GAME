//
// Created by Michal_Marszalek on 29.10.2020.
//

#ifndef RTS_GAME_FOR_EACH_H
#define RTS_GAME_FOR_EACH_H

//region STRINGIZE
#define STRINGIZE_ARG(arg) STRINGIZE(arg), arg
#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, ...)   CONCATENATE1(arg1, __VA_ARGS__)
#define CONCATENATE1(arg1, ...)  CONCATENATE2(arg1, __VA_ARGS__)
#define CONCATENATE2(arg1, ...)  arg1##__VA_ARGS__
//endregion

//region FOR_EACH
#define FOR_EACH_0(what, y, x, ...)
#define FOR_EACH_1(what, y, x, ...) what(x, y);
#define FOR_EACH_2(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_1(what, y,  __VA_ARGS__)
#define FOR_EACH_3(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_2(what, y, __VA_ARGS__)
#define FOR_EACH_4(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_3(what, y,  __VA_ARGS__)
#define FOR_EACH_5(what, y, x, ...)\
  what(x, y);\
 FOR_EACH_4(what, y,  __VA_ARGS__)
#define FOR_EACH_6(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_5(what, y,  __VA_ARGS__)
#define FOR_EACH_7(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_6(what, y,  __VA_ARGS__)
#define FOR_EACH_8(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_7(what, y,  __VA_ARGS__)
#define FOR_EACH_9(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_8(what,y,   __VA_ARGS__)
#define FOR_EACH_10(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_9(what,y,   __VA_ARGS__)
#define FOR_EACH_11(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_10(what,y,   __VA_ARGS__)
#define FOR_EACH_12(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_11(what,y,   __VA_ARGS__)
#define FOR_EACH_13(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_12(what,y,   __VA_ARGS__)
#define FOR_EACH_14(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_13(what,y,   __VA_ARGS__)
#define FOR_EACH_15(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_14(what,y,   __VA_ARGS__)
#define FOR_EACH_16(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_15(what,y,   __VA_ARGS__)
#define FOR_EACH_17(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_16(what,y,   __VA_ARGS__)
#define FOR_EACH_18(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_17(what,y,   __VA_ARGS__)
#define FOR_EACH_19(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_18(what,y,   __VA_ARGS__)
#define FOR_EACH_20(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_19(what,y,   __VA_ARGS__)
#define FOR_EACH_21(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_20(what,y,   __VA_ARGS__)
#define FOR_EACH_22(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_21(what,y,   __VA_ARGS__)
#define FOR_EACH_23(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_22(what,y,   __VA_ARGS__)
#define FOR_EACH_24(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_23(what,y,   __VA_ARGS__)
#define FOR_EACH_25(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_24(what,y,   __VA_ARGS__)
#define FOR_EACH_26(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_25(what,y,   __VA_ARGS__)
#define FOR_EACH_27(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_26(what,y,   __VA_ARGS__)
#define FOR_EACH_28(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_27(what,y,   __VA_ARGS__)
#define FOR_EACH_29(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_28(what,y,   __VA_ARGS__)
#define FOR_EACH_30(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_29(what,y,   __VA_ARGS__)
#define FOR_EACH_31(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_30(what,y,   __VA_ARGS__)
#define FOR_EACH_32(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_31(what,y,   __VA_ARGS__)
#define FOR_EACH_33(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_32(what,y,   __VA_ARGS__)
#define FOR_EACH_34(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_33(what,y,   __VA_ARGS__)
#define FOR_EACH_35(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_34(what,y,   __VA_ARGS__)
#define FOR_EACH_36(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_35(what,y,   __VA_ARGS__)
#define FOR_EACH_37(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_36(what,y,   __VA_ARGS__)
#define FOR_EACH_38(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_37(what,y,   __VA_ARGS__)
#define FOR_EACH_39(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_38(what,y,   __VA_ARGS__)
#define FOR_EACH_40(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_39(what,y,   __VA_ARGS__)
#define FOR_EACH_41(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_40(what,y,   __VA_ARGS__)
#define FOR_EACH_42(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_41(what,y,   __VA_ARGS__)
#define FOR_EACH_43(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_42(what,y,   __VA_ARGS__)
#define FOR_EACH_44(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_43(what,y,   __VA_ARGS__)
#define FOR_EACH_45(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_44(what,y,   __VA_ARGS__)
#define FOR_EACH_46(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_45(what,y,   __VA_ARGS__)
#define FOR_EACH_47(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_46(what,y,   __VA_ARGS__)
#define FOR_EACH_48(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_47(what,y,   __VA_ARGS__)
#define FOR_EACH_49(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_48(what,y,   __VA_ARGS__)
#define FOR_EACH_50(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_49(what,y,   __VA_ARGS__)
#define FOR_EACH_51(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_50(what,y,   __VA_ARGS__)
#define FOR_EACH_52(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_51(what,y,   __VA_ARGS__)
#define FOR_EACH_53(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_52(what,y,   __VA_ARGS__)
#define FOR_EACH_54(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_53(what,y,   __VA_ARGS__)
#define FOR_EACH_55(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_54(what,y,   __VA_ARGS__)
#define FOR_EACH_56(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_55(what,y,   __VA_ARGS__)
#define FOR_EACH_57(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_56(what,y,   __VA_ARGS__)
#define FOR_EACH_58(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_57(what,y,   __VA_ARGS__)
#define FOR_EACH_59(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_58(what,y,   __VA_ARGS__)
#define FOR_EACH_60(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_59(what,y,   __VA_ARGS__)
#define FOR_EACH_61(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_60(what,y,   __VA_ARGS__)
#define FOR_EACH_62(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_61(what,y,   __VA_ARGS__)
#define FOR_EACH_63(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_62(what,y,   __VA_ARGS__)
#define FOR_EACH_64(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_63(what,y,   __VA_ARGS__)
#define FOR_EACH_65(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_64(what,y,   __VA_ARGS__)
#define FOR_EACH_66(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_65(what,y,   __VA_ARGS__)
#define FOR_EACH_67(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_66(what,y,   __VA_ARGS__)
#define FOR_EACH_68(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_67(what,y,   __VA_ARGS__)
#define FOR_EACH_69(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_68(what,y,   __VA_ARGS__)
#define FOR_EACH_70(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_69(what,y,   __VA_ARGS__)
#define FOR_EACH_71(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_70(what,y,   __VA_ARGS__)
#define FOR_EACH_72(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_71(what,y,   __VA_ARGS__)
#define FOR_EACH_73(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_72(what,y,   __VA_ARGS__)
#define FOR_EACH_74(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_73(what,y,   __VA_ARGS__)
#define FOR_EACH_75(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_74(what,y,   __VA_ARGS__)
#define FOR_EACH_76(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_75(what,y,   __VA_ARGS__)
#define FOR_EACH_77(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_76(what,y,   __VA_ARGS__)
#define FOR_EACH_78(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_77(what,y,   __VA_ARGS__)
#define FOR_EACH_79(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_78(what,y,   __VA_ARGS__)
#define FOR_EACH_80(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_79(what,y,   __VA_ARGS__)
#define FOR_EACH_81(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_80(what,y,   __VA_ARGS__)
#define FOR_EACH_82(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_81(what,y,   __VA_ARGS__)
#define FOR_EACH_83(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_82(what,y,   __VA_ARGS__)
#define FOR_EACH_84(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_83(what,y,   __VA_ARGS__)
#define FOR_EACH_85(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_84(what,y,   __VA_ARGS__)
#define FOR_EACH_86(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_85(what,y,   __VA_ARGS__)
#define FOR_EACH_87(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_86(what,y,   __VA_ARGS__)
#define FOR_EACH_88(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_87(what,y,   __VA_ARGS__)
#define FOR_EACH_89(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_88(what,y,   __VA_ARGS__)
#define FOR_EACH_90(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_89(what,y,   __VA_ARGS__)
#define FOR_EACH_91(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_90(what,y,   __VA_ARGS__)
#define FOR_EACH_92(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_91(what,y,   __VA_ARGS__)
#define FOR_EACH_93(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_92(what,y,   __VA_ARGS__)
#define FOR_EACH_94(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_93(what,y,   __VA_ARGS__)
#define FOR_EACH_95(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_94(what,y,   __VA_ARGS__)
#define FOR_EACH_96(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_95(what,y,   __VA_ARGS__)
#define FOR_EACH_97(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_96(what,y,   __VA_ARGS__)
#define FOR_EACH_98(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_97(what,y,   __VA_ARGS__)
#define FOR_EACH_99(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_98(what,y,   __VA_ARGS__)
#define FOR_EACH_100(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_99(what,y,   __VA_ARGS__)
#define FOR_EACH_101(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_100(what,y,   __VA_ARGS__)
#define FOR_EACH_102(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_101(what,y,   __VA_ARGS__)
#define FOR_EACH_103(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_102(what,y,   __VA_ARGS__)
#define FOR_EACH_104(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_103(what,y,   __VA_ARGS__)
#define FOR_EACH_105(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_104(what,y,   __VA_ARGS__)
#define FOR_EACH_106(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_105(what,y,   __VA_ARGS__)
#define FOR_EACH_107(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_106(what,y,   __VA_ARGS__)
#define FOR_EACH_108(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_107(what,y,   __VA_ARGS__)
#define FOR_EACH_109(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_108(what,y,   __VA_ARGS__)
#define FOR_EACH_110(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_109(what,y,   __VA_ARGS__)
#define FOR_EACH_111(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_110(what,y,   __VA_ARGS__)
#define FOR_EACH_112(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_111(what,y,   __VA_ARGS__)
#define FOR_EACH_113(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_112(what,y,   __VA_ARGS__)
#define FOR_EACH_114(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_113(what,y,   __VA_ARGS__)
#define FOR_EACH_115(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_114(what,y,   __VA_ARGS__)
#define FOR_EACH_116(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_115(what,y,   __VA_ARGS__)
#define FOR_EACH_117(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_116(what,y,   __VA_ARGS__)
#define FOR_EACH_118(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_117(what,y,   __VA_ARGS__)
#define FOR_EACH_119(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_118(what,y,   __VA_ARGS__)
#define FOR_EACH_120(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_119(what,y,   __VA_ARGS__)
#define FOR_EACH_121(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_120(what,y,   __VA_ARGS__)
#define FOR_EACH_122(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_121(what,y,   __VA_ARGS__)
#define FOR_EACH_123(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_122(what,y,   __VA_ARGS__)
#define FOR_EACH_124(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_123(what,y,   __VA_ARGS__)
#define FOR_EACH_125(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_124(what,y,   __VA_ARGS__)
#define FOR_EACH_126(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_125(what,y,   __VA_ARGS__)
#define FOR_EACH_127(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_126(what,y,   __VA_ARGS__)
#define FOR_EACH_128(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_127(what,y,   __VA_ARGS__)
#define FOR_EACH_129(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_128(what,y,   __VA_ARGS__)
#define FOR_EACH_130(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_129(what,y,   __VA_ARGS__)
#define FOR_EACH_131(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_130(what,y,   __VA_ARGS__)
#define FOR_EACH_132(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_131(what,y,   __VA_ARGS__)
#define FOR_EACH_133(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_132(what,y,   __VA_ARGS__)
#define FOR_EACH_134(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_133(what,y,   __VA_ARGS__)
#define FOR_EACH_135(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_134(what,y,   __VA_ARGS__)
#define FOR_EACH_136(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_135(what,y,   __VA_ARGS__)
#define FOR_EACH_137(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_136(what,y,   __VA_ARGS__)
#define FOR_EACH_138(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_137(what,y,   __VA_ARGS__)
#define FOR_EACH_139(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_138(what,y,   __VA_ARGS__)
#define FOR_EACH_140(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_139(what,y,   __VA_ARGS__)
#define FOR_EACH_141(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_140(what,y,   __VA_ARGS__)
#define FOR_EACH_142(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_141(what,y,   __VA_ARGS__)
#define FOR_EACH_143(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_142(what,y,   __VA_ARGS__)
#define FOR_EACH_144(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_143(what,y,   __VA_ARGS__)
#define FOR_EACH_145(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_144(what,y,   __VA_ARGS__)
#define FOR_EACH_146(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_145(what,y,   __VA_ARGS__)
#define FOR_EACH_147(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_146(what,y,   __VA_ARGS__)
#define FOR_EACH_148(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_147(what,y,   __VA_ARGS__)
#define FOR_EACH_149(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_148(what,y,   __VA_ARGS__)
#define FOR_EACH_150(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_149(what,y,   __VA_ARGS__)
#define FOR_EACH_151(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_150(what,y,   __VA_ARGS__)
#define FOR_EACH_152(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_151(what,y,   __VA_ARGS__)
#define FOR_EACH_153(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_152(what,y,   __VA_ARGS__)
#define FOR_EACH_154(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_153(what,y,   __VA_ARGS__)
#define FOR_EACH_155(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_154(what,y,   __VA_ARGS__)
#define FOR_EACH_156(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_155(what,y,   __VA_ARGS__)
#define FOR_EACH_157(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_156(what,y,   __VA_ARGS__)
#define FOR_EACH_158(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_157(what,y,   __VA_ARGS__)
#define FOR_EACH_159(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_158(what,y,   __VA_ARGS__)
#define FOR_EACH_160(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_159(what,y,   __VA_ARGS__)
#define FOR_EACH_161(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_160(what,y,   __VA_ARGS__)
#define FOR_EACH_162(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_161(what,y,   __VA_ARGS__)
#define FOR_EACH_163(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_162(what,y,   __VA_ARGS__)
#define FOR_EACH_164(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_163(what,y,   __VA_ARGS__)
#define FOR_EACH_165(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_164(what,y,   __VA_ARGS__)
#define FOR_EACH_166(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_165(what,y,   __VA_ARGS__)
#define FOR_EACH_167(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_166(what,y,   __VA_ARGS__)
#define FOR_EACH_168(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_167(what,y,   __VA_ARGS__)
#define FOR_EACH_169(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_168(what,y,   __VA_ARGS__)
#define FOR_EACH_170(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_169(what,y,   __VA_ARGS__)
#define FOR_EACH_171(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_170(what,y,   __VA_ARGS__)
#define FOR_EACH_172(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_171(what,y,   __VA_ARGS__)
#define FOR_EACH_173(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_172(what,y,   __VA_ARGS__)
#define FOR_EACH_174(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_173(what,y,   __VA_ARGS__)
#define FOR_EACH_175(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_174(what,y,   __VA_ARGS__)
#define FOR_EACH_176(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_175(what,y,   __VA_ARGS__)
#define FOR_EACH_177(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_176(what,y,   __VA_ARGS__)
#define FOR_EACH_178(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_177(what,y,   __VA_ARGS__)
#define FOR_EACH_179(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_178(what,y,   __VA_ARGS__)
#define FOR_EACH_180(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_179(what,y,   __VA_ARGS__)
#define FOR_EACH_181(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_180(what,y,   __VA_ARGS__)
#define FOR_EACH_182(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_181(what,y,   __VA_ARGS__)
#define FOR_EACH_183(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_182(what,y,   __VA_ARGS__)
#define FOR_EACH_184(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_183(what,y,   __VA_ARGS__)
#define FOR_EACH_185(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_184(what,y,   __VA_ARGS__)
#define FOR_EACH_186(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_185(what,y,   __VA_ARGS__)
#define FOR_EACH_187(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_186(what,y,   __VA_ARGS__)
#define FOR_EACH_188(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_187(what,y,   __VA_ARGS__)
#define FOR_EACH_189(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_188(what,y,   __VA_ARGS__)
#define FOR_EACH_190(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_189(what,y,   __VA_ARGS__)
#define FOR_EACH_191(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_190(what,y,   __VA_ARGS__)
#define FOR_EACH_192(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_191(what,y,   __VA_ARGS__)
#define FOR_EACH_193(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_192(what,y,   __VA_ARGS__)
#define FOR_EACH_194(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_193(what,y,   __VA_ARGS__)
#define FOR_EACH_195(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_194(what,y,   __VA_ARGS__)
#define FOR_EACH_196(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_195(what,y,   __VA_ARGS__)
#define FOR_EACH_197(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_196(what,y,   __VA_ARGS__)
#define FOR_EACH_198(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_197(what,y,   __VA_ARGS__)
#define FOR_EACH_199(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_198(what,y,   __VA_ARGS__)
#define FOR_EACH_200(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_199(what,y,   __VA_ARGS__)
#define FOR_EACH_201(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_200(what,y,   __VA_ARGS__)
#define FOR_EACH_202(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_201(what,y,   __VA_ARGS__)
#define FOR_EACH_203(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_202(what,y,   __VA_ARGS__)
#define FOR_EACH_204(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_203(what,y,   __VA_ARGS__)
#define FOR_EACH_205(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_204(what,y,   __VA_ARGS__)
#define FOR_EACH_206(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_205(what,y,   __VA_ARGS__)
#define FOR_EACH_207(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_206(what,y,   __VA_ARGS__)
#define FOR_EACH_208(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_207(what,y,   __VA_ARGS__)
#define FOR_EACH_209(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_208(what,y,   __VA_ARGS__)
#define FOR_EACH_210(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_209(what,y,   __VA_ARGS__)
#define FOR_EACH_211(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_210(what,y,   __VA_ARGS__)
#define FOR_EACH_212(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_211(what,y,   __VA_ARGS__)
#define FOR_EACH_213(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_212(what,y,   __VA_ARGS__)
#define FOR_EACH_214(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_213(what,y,   __VA_ARGS__)
#define FOR_EACH_215(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_214(what,y,   __VA_ARGS__)
#define FOR_EACH_216(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_215(what,y,   __VA_ARGS__)
#define FOR_EACH_217(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_216(what,y,   __VA_ARGS__)
#define FOR_EACH_218(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_217(what,y,   __VA_ARGS__)
#define FOR_EACH_219(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_218(what,y,   __VA_ARGS__)
#define FOR_EACH_220(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_219(what,y,   __VA_ARGS__)
#define FOR_EACH_221(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_220(what,y,   __VA_ARGS__)
#define FOR_EACH_222(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_221(what,y,   __VA_ARGS__)
#define FOR_EACH_223(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_222(what,y,   __VA_ARGS__)
#define FOR_EACH_224(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_223(what,y,   __VA_ARGS__)
#define FOR_EACH_225(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_224(what,y,   __VA_ARGS__)
#define FOR_EACH_226(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_225(what,y,   __VA_ARGS__)
#define FOR_EACH_227(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_226(what,y,   __VA_ARGS__)
#define FOR_EACH_228(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_227(what,y,   __VA_ARGS__)
#define FOR_EACH_229(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_228(what,y,   __VA_ARGS__)
#define FOR_EACH_230(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_229(what,y,   __VA_ARGS__)
#define FOR_EACH_231(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_230(what,y,   __VA_ARGS__)
#define FOR_EACH_232(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_231(what,y,   __VA_ARGS__)
#define FOR_EACH_233(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_232(what,y,   __VA_ARGS__)
#define FOR_EACH_234(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_233(what,y,   __VA_ARGS__)
#define FOR_EACH_235(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_234(what,y,   __VA_ARGS__)
#define FOR_EACH_236(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_235(what,y,   __VA_ARGS__)
#define FOR_EACH_237(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_236(what,y,   __VA_ARGS__)
#define FOR_EACH_238(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_237(what,y,   __VA_ARGS__)
#define FOR_EACH_239(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_238(what,y,   __VA_ARGS__)
#define FOR_EACH_240(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_239(what,y,   __VA_ARGS__)
#define FOR_EACH_241(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_240(what,y,   __VA_ARGS__)
#define FOR_EACH_242(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_241(what,y,   __VA_ARGS__)
#define FOR_EACH_243(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_242(what,y,   __VA_ARGS__)
#define FOR_EACH_244(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_243(what,y,   __VA_ARGS__)
#define FOR_EACH_245(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_244(what,y,   __VA_ARGS__)
#define FOR_EACH_246(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_245(what,y,   __VA_ARGS__)
#define FOR_EACH_247(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_246(what,y,   __VA_ARGS__)
#define FOR_EACH_248(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_247(what,y,   __VA_ARGS__)
#define FOR_EACH_249(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_248(what,y,   __VA_ARGS__)
#define FOR_EACH_250(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_249(what,y,   __VA_ARGS__)
#define FOR_EACH_251(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_250(what,y,   __VA_ARGS__)
#define FOR_EACH_252(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_251(what,y,   __VA_ARGS__)
#define FOR_EACH_253(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_252(what,y,   __VA_ARGS__)
#define FOR_EACH_254(what, y, x, ...)\
  what(x, y);\
  FOR_EACH_253(what,y,   __VA_ARGS__)
//endregion

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__ __VA_OPT__(,) FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, _138, _139, _140, _141, _142, _143, _144, _145, _146, _147, _148, _149, _150, _151, _152, _153, _154, _155, _156, _157, _158, _159, _160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, _171, _172, _173, _174, _175, _176, _177, _178, _179, _180, _181, _182, _183, _184, _185, _186, _187, _188, _189, _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200, _201, _202, _203, _204, _205, _206, _207, _208, _209, _210, _211, _212, _213, _214, _215, _216, _217, _218, _219, _220, _221, _222, _223, _224, _225, _226, _227, _228, _229, _230, _231, _232, _233, _234, _235, _236, _237, _238, _239, _240, _241, _242, _243, _244, _245, _246, _247, _248, _249, _250, _251, _252, _253, _254, N, ...) N
#define FOR_EACH_RSEQ_N() 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, x, ...) CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...) FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, x, ## __VA_ARGS__)


#endif //RTS_GAME_FOR_EACH_H
