#include "librecomp/recomp.h"

void Player_ProcessItemButtons(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    unsigned int rounding_mode = DEFAULT_ROUNDING_MODE;
    int c1cs = 0;
    // 0x8082FE0C: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x8082FE10: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x8082FE14: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8082FE18: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8082FE1C: sw          $a1, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r5;
    // 0x8082FE20: lw          $t6, 0xA6C($s0)
    ctx->r14 = MEM_W(0XA6C, ctx->r16);
    // 0x8082FE24: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x8082FE28: ori         $at, $at, 0x800
    ctx->r1 = ctx->r1 | 0X800;
    // 0x8082FE2C: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x8082FE30: bnel        $t7, $zero, L_808302BC
    if (ctx->r15 != 0) {
        // 0x8082FE34: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(0X34, ctx->r29);
        goto L_808302BC;
    }
    goto skip_0;
    // 0x8082FE34: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    skip_0:
    // 0x8082FE38: lw          $t8, 0xA70($s0)
    ctx->r24 = MEM_W(0XA70, ctx->r16);
    // 0x8082FE3C: sll         $t9, $t8, 6
    ctx->r25 = S32(ctx->r24) << 6;
    // 0x8082FE40: bltzl       $t9, L_808302BC
    if (SIGNED(ctx->r25) < 0) {
        // 0x8082FE44: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(0X34, ctx->r29);
        goto L_808302BC;
    }
    goto skip_1;
    // 0x8082FE44: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    skip_1:
    // 0x8082FE48: lw          $t2, 0xA74($s0)
    ctx->r10 = MEM_W(0XA74, ctx->r16);
    // 0x8082FE4C: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10) << 2;
    // 0x8082FE50: bltzl       $t3, L_808302BC
    if (SIGNED(ctx->r11) < 0) {
        // 0x8082FE54: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(0X34, ctx->r29);
        goto L_808302BC;
    }
    goto skip_2;
    // 0x8082FE54: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    skip_2:
    // 0x8082FE58: jal         0x801240DC
    // 0x8082FE5C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_801240DC(rdram, ctx);
    goto after_0;
    // 0x8082FE5C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x8082FE60: bnel        $v0, $zero, L_808302BC
    if (ctx->r2 != 0) {
        // 0x8082FE64: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(0X34, ctx->r29);
        goto L_808302BC;
    }
    goto skip_3;
    // 0x8082FE64: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    skip_3:
    // 0x8082FE68: lbu         $t4, 0x14B($s0)
    ctx->r12 = MEM_BU(0X14B, ctx->r16);
    // 0x8082FE6C: addiu       $t1, $zero, 0x4
    ctx->r9 = ADD32(0, 0X4);
    // 0x8082FE70: bnel        $t1, $t4, L_8082FF68
    if (ctx->r9 != ctx->r12) {
        // 0x8082FE74: lh          $t3, 0x0($s0)
        ctx->r11 = MEM_H(0X0, ctx->r16);
        goto L_8082FF68;
    }
    goto skip_4;
    // 0x8082FE74: lh          $t3, 0x0($s0)
    ctx->r11 = MEM_H(0X0, ctx->r16);
    skip_4:
    // 0x8082FE78: lbu         $v0, 0x153($s0)
    ctx->r2 = MEM_BU(0X153, ctx->r16);
    // 0x8082FE7C: beq         $v0, $zero, L_8082FF64
    if (ctx->r2 == 0) {
        // 0x8082FE80: addiu       $a1, $v0, 0x39
        ctx->r5 = ADD32(ctx->r2, 0X39);
        goto L_8082FF64;
    }
    // 0x8082FE80: addiu       $a1, $v0, 0x39
    ctx->r5 = ADD32(ctx->r2, 0X39);
    // 0x8082FE84: jal         0x8082FD0C
    // 0x8082FE88: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8082FD0C(rdram, ctx);
    goto after_1;
    // 0x8082FE88: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x8082FE8C: addiu       $t1, $zero, 0x4
    ctx->r9 = ADD32(0, 0X4);
    // 0x8082FE90: bgez        $v0, L_8082FF2C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x8082FE94: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
        goto L_8082FF2C;
    }
    // 0x8082FE94: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8082FE98: lb          $v1, 0x154($s0)
    ctx->r3 = MEM_B(0X154, ctx->r16);
    // 0x8082FE9C: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x8082FEA0: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x8082FEA4: bne         $v1, $zero, L_8082FEDC
    if (ctx->r3 != 0) {
        // 0x8082FEA8: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
        goto L_8082FEDC;
    }
    // 0x8082FEA8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8082FEAC: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x8082FEB0: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x8082FEB4: lbu         $v0, 0x20($t0)
    ctx->r2 = MEM_BU(0X20, ctx->r8);
    // 0x8082FEB8: bne         $t1, $v0, L_8082FEC8
    if (ctx->r9 != ctx->r2) {
        // 0x8082FEBC: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
        goto L_8082FEC8;
    }
    // 0x8082FEBC: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x8082FEC0: b           L_8082FEC8
    // 0x8082FEC4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    goto L_8082FEC8;
    // 0x8082FEC4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
L_8082FEC8:
    // 0x8082FEC8: sll         $t5, $a1, 2
    ctx->r13 = S32(ctx->r5) << 2;
    // 0x8082FECC: addu        $t6, $t0, $t5
    ctx->r14 = ADD32(ctx->r8, ctx->r13);
    // 0x8082FED0: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x8082FED4: b           L_8082FEE4
    // 0x8082FED8: lbu         $a2, 0x4C($t7)
    ctx->r6 = MEM_BU(0X4C, ctx->r15);
    goto L_8082FEE4;
    // 0x8082FED8: lbu         $a2, 0x4C($t7)
    ctx->r6 = MEM_BU(0X4C, ctx->r15);
L_8082FEDC:
    // 0x8082FEDC: addu        $t8, $t0, $v1
    ctx->r24 = ADD32(ctx->r8, ctx->r3);
    // 0x8082FEE0: lbu         $a2, 0x4C($t8)
    ctx->r6 = MEM_BU(0X4C, ctx->r24);
L_8082FEE4:
    // 0x8082FEE4: jal         0x8082F524
    // 0x8082FEE8: andi        $a1, $a2, 0xFF
    ctx->r5 = ctx->r6 & 0XFF;
    Player_ItemToItemAction(rdram, ctx);
    goto after_2;
    // 0x8082FEE8: andi        $a1, $a2, 0xFF
    ctx->r5 = ctx->r6 & 0XFF;
    after_2:
    // 0x8082FEEC: addiu       $v1, $v0, -0x3A
    ctx->r3 = ADD32(ctx->r2, -0X3A);
    // 0x8082FEF0: bltz        $v1, L_8082FF04
    if (SIGNED(ctx->r3) < 0) {
        // 0x8082FEF4: or          $a0, $v1, $zero
        ctx->r4 = ctx->r3 | 0;
        goto L_8082FF04;
    }
    // 0x8082FEF4: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
    // 0x8082FEF8: slti        $at, $v1, 0x18
    ctx->r1 = SIGNED(ctx->r3) < 0X18 ? 1 : 0;
    // 0x8082FEFC: bne         $at, $zero, L_8082FF0C
    if (ctx->r1 != 0) {
        // 0x8082FF00: nop
    
        goto L_8082FF0C;
    }
    // 0x8082FF00: nop

L_8082FF04:
    // 0x8082FF04: lbu         $a0, 0x153($s0)
    ctx->r4 = MEM_BU(0X153, ctx->r16);
    // 0x8082FF08: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
L_8082FF0C:
    // 0x8082FF0C: jal         0x80122ED8
    // 0x8082FF10: nop

    Player_MaskIdToItemId(rdram, ctx);
    goto after_3;
    // 0x8082FF10: nop

    after_3:
    // 0x8082FF14: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x8082FF18: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8082FF1C: jal         0x80831990
    // 0x8082FF20: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    Player_UseItem(rdram, ctx);
    goto after_4;
    // 0x8082FF20: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    after_4:
    // 0x8082FF24: b           L_808302BC
    // 0x8082FF28: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_808302BC;
    // 0x8082FF28: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_8082FF2C:
    // 0x8082FF2C: lbu         $t9, 0x153($s0)
    ctx->r25 = MEM_BU(0X153, ctx->r16);
    // 0x8082FF30: addiu       $at, $zero, 0x14
    ctx->r1 = ADD32(0, 0X14);
    // 0x8082FF34: bne         $t9, $at, L_8082FF60
    if (ctx->r25 != ctx->r1) {
        // 0x8082FF38: lui         $t0, 0x801F
        ctx->r8 = S32(0X801F << 16);
        goto L_8082FF60;
    }
    // 0x8082FF38: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x8082FF3C: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x8082FF40: lb          $t2, 0x39($t0)
    ctx->r10 = MEM_B(0X39, ctx->r8);
    // 0x8082FF44: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x8082FF48: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8082FF4C: bnel        $t2, $zero, L_8082FF64
    if (ctx->r10 != 0) {
        // 0x8082FF50: sb          $v1, 0x154($s0)
        MEM_B(0X154, ctx->r16) = ctx->r3;
        goto L_8082FF64;
    }
    goto skip_5;
    // 0x8082FF50: sb          $v1, 0x154($s0)
    MEM_B(0X154, ctx->r16) = ctx->r3;
    skip_5:
    // 0x8082FF54: jal         0x80838A20
    // 0x8082FF58: sw          $v1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r3;
    func_80838A20(rdram, ctx);
    goto after_5;
    // 0x8082FF58: sw          $v1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r3;
    after_5:
    // 0x8082FF5C: lw          $v1, 0x48($sp)
    ctx->r3 = MEM_W(0X48, ctx->r29);
L_8082FF60:
    // 0x8082FF60: sb          $v1, 0x154($s0)
    MEM_B(0X154, ctx->r16) = ctx->r3;
L_8082FF64:
    // 0x8082FF64: lh          $t3, 0x0($s0)
    ctx->r11 = MEM_H(0X0, ctx->r16);
L_8082FF68:
    // 0x8082FF68: bne         $t3, $zero, L_808300D4
    if (ctx->r11 != 0) {
        // 0x8082FF6C: nop
    
        goto L_808300D4;
    }
    // 0x8082FF6C: nop

    // 0x8082FF70: lb          $t4, 0x14A($s0)
    ctx->r12 = MEM_B(0X14A, ctx->r16);
    // 0x8082FF74: slti        $at, $t4, 0x2
    ctx->r1 = SIGNED(ctx->r12) < 0X2 ? 1 : 0;
    // 0x8082FF78: bne         $at, $zero, L_808300D4
    if (ctx->r1 != 0) {
        // 0x8082FF7C: nop
    
        goto L_808300D4;
    }
    // 0x8082FF7C: nop

    // 0x8082FF80: jal         0x80124148
    // 0x8082FF84: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Player_GetHeldBButtonSword(rdram, ctx);
    goto after_6;
    // 0x8082FF84: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_6:
    // 0x8082FF88: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x8082FF8C: beq         $v0, $zero, L_8082FF9C
    if (ctx->r2 == 0) {
        // 0x8082FF90: addiu       $t0, $t0, -0x990
        ctx->r8 = ADD32(ctx->r8, -0X990);
        goto L_8082FF9C;
    }
    // 0x8082FF90: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x8082FF94: lhu         $t5, 0x1016($t0)
    ctx->r13 = MEM_HU(0X1016, ctx->r8);
    // 0x8082FF98: bne         $t5, $zero, L_808300BC
    if (ctx->r13 != 0) {
        // 0x8082FF9C: lui         $t6, 0x801F
        ctx->r14 = S32(0X801F << 16);
        goto L_808300BC;
    }
L_8082FF9C:
    // 0x8082FF9C: lui         $t6, 0x801F
    ctx->r14 = S32(0X801F << 16);
    // 0x8082FFA0: lw          $t6, 0x3F60($t6)
    ctx->r14 = MEM_W(0X3F60, ctx->r14);
    // 0x8082FFA4: lh          $t7, 0x6D6($t6)
    ctx->r15 = MEM_H(0X6D6, ctx->r14);
    // 0x8082FFA8: beq         $t7, $zero, L_8082FFB8
    if (ctx->r15 == 0) {
        // 0x8082FFAC: nop
    
        goto L_8082FFB8;
    }
    // 0x8082FFAC: nop

    // 0x8082FFB0: b           L_8082FFC4
    // 0x8082FFB4: addiu       $a2, $zero, 0x5C
    ctx->r6 = ADD32(0, 0X5C);
    goto L_8082FFC4;
    // 0x8082FFB4: addiu       $a2, $zero, 0x5C
    ctx->r6 = ADD32(0, 0X5C);
L_8082FFB8:
    // 0x8082FFB8: jal         0x8012EC80
    // 0x8082FFBC: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    Inventory_GetBtnBItem(rdram, ctx);
    goto after_7;
    // 0x8082FFBC: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    after_7:
    // 0x8082FFC0: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
L_8082FFC4:
    // 0x8082FFC4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8082FFC8: jal         0x8082FC78
    // 0x8082FFCC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    Player_ItemIsInUse(rdram, ctx);
    goto after_8;
    // 0x8082FFCC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_8:
    // 0x8082FFD0: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x8082FFD4: bne         $v0, $zero, L_808300D4
    if (ctx->r2 != 0) {
        // 0x8082FFD8: addiu       $t0, $t0, -0x990
        ctx->r8 = ADD32(ctx->r8, -0X990);
        goto L_808300D4;
    }
    // 0x8082FFD8: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x8082FFDC: lbu         $t8, 0x3F19($t0)
    ctx->r24 = MEM_BU(0X3F19, ctx->r8);
    // 0x8082FFE0: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x8082FFE4: beql        $t8, $at, L_8082FFF8
    if (ctx->r24 == ctx->r1) {
        // 0x8082FFE8: lhu         $t9, 0x3F22($t0)
        ctx->r25 = MEM_HU(0X3F22, ctx->r8);
        goto L_8082FFF8;
    }
    goto skip_6;
    // 0x8082FFE8: lhu         $t9, 0x3F22($t0)
    ctx->r25 = MEM_HU(0X3F22, ctx->r8);
    skip_6:
    // 0x8082FFEC: b           L_80830014
    // 0x8082FFF0: lbu         $a1, 0x4D($t0)
    ctx->r5 = MEM_BU(0X4D, ctx->r8);
    goto L_80830014;
    // 0x8082FFF0: lbu         $a1, 0x4D($t0)
    ctx->r5 = MEM_BU(0X4D, ctx->r8);
    // 0x8082FFF4: lhu         $t9, 0x3F22($t0)
    ctx->r25 = MEM_HU(0X3F22, ctx->r8);
L_8082FFF8:
    // 0x8082FFF8: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x8082FFFC: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x80830000: bne         $t9, $at, L_80830010
    if (ctx->r25 != ctx->r1) {
        // 0x80830004: nop
    
        goto L_80830010;
    }
    // 0x80830004: nop

    // 0x80830008: b           L_80830010
    // 0x8083000C: lbu         $v0, 0x4D($t0)
    ctx->r2 = MEM_BU(0X4D, ctx->r8);
    goto L_80830010;
    // 0x8083000C: lbu         $v0, 0x4D($t0)
    ctx->r2 = MEM_BU(0X4D, ctx->r8);
L_80830010:
    // 0x80830010: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
L_80830014:
    // 0x80830014: jal         0x8082FC78
    // 0x80830018: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Player_ItemIsInUse(rdram, ctx);
    goto after_9;
    // 0x80830018: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_9:
    // 0x8083001C: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x80830020: bne         $v0, $zero, L_808300D4
    if (ctx->r2 != 0) {
        // 0x80830024: addiu       $t0, $t0, -0x990
        ctx->r8 = ADD32(ctx->r8, -0X990);
        goto L_808300D4;
    }
    // 0x80830024: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x80830028: lbu         $t2, 0x3F1A($t0)
    ctx->r10 = MEM_BU(0X3F1A, ctx->r8);
    // 0x8083002C: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x80830030: beql        $t2, $at, L_80830044
    if (ctx->r10 == ctx->r1) {
        // 0x80830034: lhu         $t3, 0x3F22($t0)
        ctx->r11 = MEM_HU(0X3F22, ctx->r8);
        goto L_80830044;
    }
    goto skip_7;
    // 0x80830034: lhu         $t3, 0x3F22($t0)
    ctx->r11 = MEM_HU(0X3F22, ctx->r8);
    skip_7:
    // 0x80830038: b           L_80830060
    // 0x8083003C: lbu         $a1, 0x4E($t0)
    ctx->r5 = MEM_BU(0X4E, ctx->r8);
    goto L_80830060;
    // 0x8083003C: lbu         $a1, 0x4E($t0)
    ctx->r5 = MEM_BU(0X4E, ctx->r8);
    // 0x80830040: lhu         $t3, 0x3F22($t0)
    ctx->r11 = MEM_HU(0X3F22, ctx->r8);
L_80830044:
    // 0x80830044: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x80830048: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x8083004C: bne         $t3, $at, L_8083005C
    if (ctx->r11 != ctx->r1) {
        // 0x80830050: nop
    
        goto L_8083005C;
    }
    // 0x80830050: nop

    // 0x80830054: b           L_8083005C
    // 0x80830058: lbu         $v0, 0x4E($t0)
    ctx->r2 = MEM_BU(0X4E, ctx->r8);
    goto L_8083005C;
    // 0x80830058: lbu         $v0, 0x4E($t0)
    ctx->r2 = MEM_BU(0X4E, ctx->r8);
L_8083005C:
    // 0x8083005C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
L_80830060:
    // 0x80830060: jal         0x8082FC78
    // 0x80830064: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Player_ItemIsInUse(rdram, ctx);
    goto after_10;
    // 0x80830064: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_10:
    // 0x80830068: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x8083006C: bne         $v0, $zero, L_808300D4
    if (ctx->r2 != 0) {
        // 0x80830070: addiu       $t0, $t0, -0x990
        ctx->r8 = ADD32(ctx->r8, -0X990);
        goto L_808300D4;
    }
    // 0x80830070: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x80830074: lbu         $t4, 0x3F1B($t0)
    ctx->r12 = MEM_BU(0X3F1B, ctx->r8);
    // 0x80830078: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x8083007C: beql        $t4, $at, L_80830090
    if (ctx->r12 == ctx->r1) {
        // 0x80830080: lhu         $t5, 0x3F22($t0)
        ctx->r13 = MEM_HU(0X3F22, ctx->r8);
        goto L_80830090;
    }
    goto skip_8;
    // 0x80830080: lhu         $t5, 0x3F22($t0)
    ctx->r13 = MEM_HU(0X3F22, ctx->r8);
    skip_8:
    // 0x80830084: b           L_808300AC
    // 0x80830088: lbu         $a1, 0x4F($t0)
    ctx->r5 = MEM_BU(0X4F, ctx->r8);
    goto L_808300AC;
    // 0x80830088: lbu         $a1, 0x4F($t0)
    ctx->r5 = MEM_BU(0X4F, ctx->r8);
    // 0x8083008C: lhu         $t5, 0x3F22($t0)
    ctx->r13 = MEM_HU(0X3F22, ctx->r8);
L_80830090:
    // 0x80830090: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x80830094: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x80830098: bne         $t5, $at, L_808300A8
    if (ctx->r13 != ctx->r1) {
        // 0x8083009C: nop
    
        goto L_808300A8;
    }
    // 0x8083009C: nop

    // 0x808300A0: b           L_808300A8
    // 0x808300A4: lbu         $v0, 0x4F($t0)
    ctx->r2 = MEM_BU(0X4F, ctx->r8);
    goto L_808300A8;
    // 0x808300A4: lbu         $v0, 0x4F($t0)
    ctx->r2 = MEM_BU(0X4F, ctx->r8);
L_808300A8:
    // 0x808300A8: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
L_808300AC:
    // 0x808300AC: jal         0x8082FC78
    // 0x808300B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Player_ItemIsInUse(rdram, ctx);
    goto after_11;
    // 0x808300B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_11:
    // 0x808300B4: bne         $v0, $zero, L_808300D4
    if (ctx->r2 != 0) {
        // 0x808300B8: nop
    
        goto L_808300D4;
    }
    // 0x808300B8: nop

L_808300BC:
    // 0x808300BC: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x808300C0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x808300C4: jal         0x80831990
    // 0x808300C8: addiu       $a2, $zero, 0xFF
    ctx->r6 = ADD32(0, 0XFF);
    Player_UseItem(rdram, ctx);
    goto after_12;
    // 0x808300C8: addiu       $a2, $zero, 0xFF
    ctx->r6 = ADD32(0, 0XFF);
    after_12:
    // 0x808300CC: b           L_808302BC
    // 0x808300D0: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_808302BC;
    // 0x808300D0: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_808300D4:
    // 0x808300D4: jal         0x8082FDC4
    // 0x808300D8: nop

    func_8082FDC4(rdram, ctx);
    goto after_13;
    // 0x808300D8: nop

    after_13:
    // 0x808300DC: slti        $at, $v0, 0x4
    ctx->r1 = SIGNED(ctx->r2) < 0X4 ? 1 : 0;
    // 0x808300E0: bne         $at, $zero, L_80830108
    if (ctx->r1 != 0) {
        // 0x808300E4: or          $a3, $v0, $zero
        ctx->r7 = ctx->r2 | 0;
        goto L_80830108;
    }
    // 0x808300E4: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x808300E8: lbu         $t6, 0x14B($s0)
    ctx->r14 = MEM_BU(0X14B, ctx->r16);
    // 0x808300EC: bnel        $t6, $zero, L_8083010C
    if (ctx->r14 != 0) {
        // 0x808300F0: lw          $a0, 0x54($sp)
        ctx->r4 = MEM_W(0X54, ctx->r29);
        goto L_8083010C;
    }
    goto skip_9;
    // 0x808300F0: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    skip_9:
    // 0x808300F4: lb          $t7, 0x147($s0)
    ctx->r15 = MEM_B(0X147, ctx->r16);
    // 0x808300F8: addiu       $at, $zero, 0x6
    ctx->r1 = ADD32(0, 0X6);
    // 0x808300FC: beql        $t7, $at, L_8083010C
    if (ctx->r15 == ctx->r1) {
        // 0x80830100: lw          $a0, 0x54($sp)
        ctx->r4 = MEM_W(0X54, ctx->r29);
        goto L_8083010C;
    }
    goto skip_10;
    // 0x80830100: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    skip_10:
    // 0x80830104: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
L_80830108:
    // 0x80830108: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
L_8083010C:
    // 0x8083010C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80830110: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x80830114: jal         0x8012364C
    // 0x80830118: sw          $a3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r7;
    Player_GetItemOnButton(rdram, ctx);
    goto after_14;
    // 0x80830118: sw          $a3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r7;
    after_14:
    // 0x8083011C: slti        $at, $v0, 0xFD
    ctx->r1 = SIGNED(ctx->r2) < 0XFD ? 1 : 0;
    // 0x80830120: lw          $a3, 0x38($sp)
    ctx->r7 = MEM_W(0X38, ctx->r29);
    // 0x80830124: addiu       $t1, $zero, 0x4
    ctx->r9 = ADD32(0, 0X4);
    // 0x80830128: bne         $at, $zero, L_808301A8
    if (ctx->r1 != 0) {
        // 0x8083012C: or          $a2, $v0, $zero
        ctx->r6 = ctx->r2 | 0;
        goto L_808301A8;
    }
    // 0x8083012C: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x80830130: lui         $t8, 0x8086
    ctx->r24 = S32(RELOC_HI16(9, 0X350B4) << 16);
    // 0x80830134: lw          $t8, 0x2B44($t8)
    ctx->r24 = MEM_W((int16_t)RELOC_LO16(9, 0X350B4), ctx->r24);
    // 0x80830138: lui         $v0, 0x8086
    ctx->r2 = S32(RELOC_HI16(9, 0X2F518) << 16);
    // 0x8083013C: addiu       $v0, $v0, -0x3058
    ctx->r2 = ADD32(ctx->r2, (int16_t)RELOC_LO16(9, 0X2F518));
    // 0x80830140: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x80830144: lhu         $v1, 0x0($t8)
    ctx->r3 = MEM_HU(0X0, ctx->r24);
L_80830148:
    // 0x80830148: lhu         $t9, 0x0($v0)
    ctx->r25 = MEM_HU(0X0, ctx->r2);
    // 0x8083014C: nor         $t2, $t9, $zero
    ctx->r10 = ~(ctx->r25 | 0);
    // 0x80830150: nor         $t3, $t2, $v1
    ctx->r11 = ~(ctx->r10 | ctx->r3);
    // 0x80830154: beql        $t3, $zero, L_8083016C
    if (ctx->r11 == 0) {
        // 0x80830158: lw          $a0, 0x54($sp)
        ctx->r4 = MEM_W(0X54, ctx->r29);
        goto L_8083016C;
    }
    goto skip_11;
    // 0x80830158: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    skip_11:
    // 0x8083015C: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    // 0x80830160: bne         $a3, $t1, L_80830148
    if (ctx->r7 != ctx->r9) {
        // 0x80830164: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
        goto L_80830148;
    }
    // 0x80830164: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80830168: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
L_8083016C:
    // 0x8083016C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80830170: jal         0x8012364C
    // 0x80830174: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    Player_GetItemOnButton(rdram, ctx);
    goto after_15;
    // 0x80830174: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    after_15:
    // 0x80830178: slti        $at, $v0, 0xFD
    ctx->r1 = SIGNED(ctx->r2) < 0XFD ? 1 : 0;
    // 0x8083017C: beq         $at, $zero, L_808302B8
    if (ctx->r1 == 0) {
        // 0x80830180: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
        goto L_808302B8;
    }
    // 0x80830180: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80830184: jal         0x8082F524
    // 0x80830188: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    Player_ItemToItemAction(rdram, ctx);
    goto after_16;
    // 0x80830188: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_16:
    // 0x8083018C: lb          $t4, 0x147($s0)
    ctx->r12 = MEM_B(0X147, ctx->r16);
    // 0x80830190: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80830194: lui         $at, 0x8086
    ctx->r1 = S32(RELOC_HI16(9, 0X350BC) << 16);
    // 0x80830198: bnel        $v0, $t4, L_808302BC
    if (ctx->r2 != ctx->r12) {
        // 0x8083019C: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(0X34, ctx->r29);
        goto L_808302BC;
    }
    goto skip_12;
    // 0x8083019C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    skip_12:
    // 0x808301A0: b           L_808302B8
    // 0x808301A4: sw          $t5, 0x2B4C($at)
    MEM_W((int16_t)RELOC_LO16(9, 0X350BC), ctx->r1) = ctx->r13;
    goto L_808302B8;
    // 0x808301A4: sw          $t5, 0x2B4C($at)
    MEM_W((int16_t)RELOC_LO16(9, 0X350BC), ctx->r1) = ctx->r13;
L_808301A8:
    // 0x808301A8: addiu       $at, $zero, 0xF0
    ctx->r1 = ADD32(0, 0XF0);
    // 0x808301AC: bnel        $v0, $at, L_8083020C
    if (ctx->r2 != ctx->r1) {
        // 0x808301B0: addiu       $at, $zero, 0xF1
        ctx->r1 = ADD32(0, 0XF1);
        goto L_8083020C;
    }
    goto skip_13;
    // 0x808301B0: addiu       $at, $zero, 0xF1
    ctx->r1 = ADD32(0, 0XF1);
    skip_13:
    // 0x808301B4: lhu         $t6, 0xB60($s0)
    ctx->r14 = MEM_HU(0XB60, ctx->r16);
    // 0x808301B8: lw          $a1, 0x54($sp)
    ctx->r5 = MEM_W(0X54, ctx->r29);
    // 0x808301BC: bne         $t6, $zero, L_808302B8
    if (ctx->r14 != 0) {
        // 0x808301C0: addiu       $a0, $a1, 0x1CA0
        ctx->r4 = ADD32(ctx->r5, 0X1CA0);
        goto L_808302B8;
    }
    // 0x808301C0: addiu       $a0, $a1, 0x1CA0
    ctx->r4 = ADD32(ctx->r5, 0X1CA0);
    // 0x808301C4: lwc1        $f4, 0x40($s0)
    ctx->f4.u32l = MEM_W(0X40, ctx->r16);
    // 0x808301C8: lw          $a3, 0x3C($s0)
    ctx->r7 = MEM_W(0X3C, ctx->r16);
    // 0x808301CC: addiu       $a2, $zero, 0x9
    ctx->r6 = ADD32(0, 0X9);
    // 0x808301D0: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    // 0x808301D4: lwc1        $f6, 0x44($s0)
    ctx->f6.u32l = MEM_W(0X44, ctx->r16);
    // 0x808301D8: sw          $zero, 0x24($sp)
    MEM_W(0X24, ctx->r29) = 0;
    // 0x808301DC: sw          $zero, 0x20($sp)
    MEM_W(0X20, ctx->r29) = 0;
    // 0x808301E0: sw          $zero, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = 0;
    // 0x808301E4: sw          $zero, 0x18($sp)
    MEM_W(0X18, ctx->r29) = 0;
    // 0x808301E8: jal         0x800BAC60
    // 0x808301EC: swc1        $f6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f6.u32l;
    Actor_Spawn(rdram, ctx);
    goto after_17;
    // 0x808301EC: swc1        $f6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f6.u32l;
    after_17:
    // 0x808301F0: beq         $v0, $zero, L_808302B8
    if (ctx->r2 == 0) {
        // 0x808301F4: addiu       $t7, $zero, 0x136
        ctx->r15 = ADD32(0, 0X0);
        goto L_808302B8;
    }
    // 0x808301F4: addiu       $t7, $zero, 0x136
    ctx->r15 = ADD32(0, 0X0);
    // 0x808301F8: sh          $zero, 0x1F0($v0)
    MEM_H(0X1F0, ctx->r2) = 0;
    // 0x808301FC: sh          $t7, 0xB60($s0)
    MEM_H(0XB60, ctx->r16) = ctx->r15;
    // 0x80830200: b           L_808302BC
    // 0x80830204: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_808302BC;
    // 0x80830204: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    // 0x80830208: addiu       $at, $zero, 0xF1
    ctx->r1 = ADD32(0, 0XF1);
L_8083020C:
    // 0x8083020C: bne         $v0, $at, L_80830224
    if (ctx->r2 != ctx->r1) {
        // 0x80830210: lw          $a0, 0x54($sp)
        ctx->r4 = MEM_W(0X54, ctx->r29);
        goto L_80830224;
    }
    // 0x80830210: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x80830214: jal         0x80839978
    // 0x80830218: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80839978(rdram, ctx);
    goto after_18;
    // 0x80830218: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_18:
    // 0x8083021C: b           L_808302BC
    // 0x80830220: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_808302BC;
    // 0x80830220: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_80830224:
    // 0x80830224: addiu       $at, $zero, 0xF2
    ctx->r1 = ADD32(0, 0XF2);
    // 0x80830228: bne         $v0, $at, L_80830244
    if (ctx->r2 != ctx->r1) {
        // 0x8083022C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
        goto L_80830244;
    }
    // 0x8083022C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80830230: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x80830234: jal         0x80839A10
    // 0x80830238: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80839A10(rdram, ctx);
    goto after_19;
    // 0x80830238: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_19:
    // 0x8083023C: b           L_808302BC
    // 0x80830240: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_808302BC;
    // 0x80830240: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_80830244:
    // 0x80830244: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x80830248: sw          $a2, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r6;
    // 0x8083024C: jal         0x8082F524
    // 0x80830250: sw          $a3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r7;
    Player_ItemToItemAction(rdram, ctx);
    goto after_20;
    // 0x80830250: sw          $a3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r7;
    after_20:
    // 0x80830254: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80830258: jal         0x80124110
    // 0x8083025C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    Player_BButtonSwordFromIA(rdram, ctx);
    goto after_21;
    // 0x8083025C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_21:
    // 0x80830260: lui         $t0, 0x801F
    ctx->r8 = S32(0X801F << 16);
    // 0x80830264: addiu       $t0, $t0, -0x990
    ctx->r8 = ADD32(ctx->r8, -0X990);
    // 0x80830268: lw          $a2, 0x3C($sp)
    ctx->r6 = MEM_W(0X3C, ctx->r29);
    // 0x8083026C: beq         $v0, $zero, L_808302A8
    if (ctx->r2 == 0) {
        // 0x80830270: lw          $a3, 0x38($sp)
        ctx->r7 = MEM_W(0X38, ctx->r29);
        goto L_808302A8;
    }
    // 0x80830270: lw          $a3, 0x38($sp)
    ctx->r7 = MEM_W(0X38, ctx->r29);
    // 0x80830274: lhu         $t8, 0x1016($t0)
    ctx->r24 = MEM_HU(0X1016, ctx->r8);
    // 0x80830278: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x8083027C: beql        $t8, $zero, L_808302AC
    if (ctx->r24 == 0) {
        // 0x80830280: sb          $a3, 0x146($s0)
        MEM_B(0X146, ctx->r16) = ctx->r7;
        goto L_808302AC;
    }
    goto skip_14;
    // 0x80830280: sb          $a3, 0x146($s0)
    MEM_B(0X146, ctx->r16) = ctx->r7;
    skip_14:
    // 0x80830284: jal         0x80152498
    // 0x80830288: addiu       $a0, $a0, 0x4908
    ctx->r4 = ADD32(ctx->r4, 0X4908);
    Message_GetState(rdram, ctx);
    goto after_22;
    // 0x80830288: addiu       $a0, $a0, 0x4908
    ctx->r4 = ADD32(ctx->r4, 0X4908);
    after_22:
    // 0x8083028C: bne         $v0, $zero, L_808302B8
    if (ctx->r2 != 0) {
        // 0x80830290: lw          $a0, 0x54($sp)
        ctx->r4 = MEM_W(0X54, ctx->r29);
        goto L_808302B8;
    }
    // 0x80830290: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x80830294: addiu       $a1, $zero, 0xF7
    ctx->r5 = ADD32(0, 0XF7);
    // 0x80830298: jal         0x801518B0
    // 0x8083029C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    Message_StartTextbox(rdram, ctx);
    goto after_23;
    // 0x8083029C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_23:
    // 0x808302A0: b           L_808302BC
    // 0x808302A4: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_808302BC;
    // 0x808302A4: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_808302A8:
    // 0x808302A8: sb          $a3, 0x146($s0)
    MEM_B(0X146, ctx->r16) = ctx->r7;
L_808302AC:
    // 0x808302AC: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(0X54, ctx->r29);
    // 0x808302B0: jal         0x80831990
    // 0x808302B4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    Player_UseItem(rdram, ctx);
    goto after_24;
    // 0x808302B4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_24:
L_808302B8:
    // 0x808302B8: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_808302BC:
    // 0x808302BC: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(0X30, ctx->r29);
    // 0x808302C0: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    // 0x808302C4: jr          $ra
    // 0x808302C8: nop

    return;
    // 0x808302C8: nop

;}
