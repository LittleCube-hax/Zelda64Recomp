#include "librecomp/recomp.h"

void Play_Init(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    unsigned int rounding_mode = DEFAULT_ROUNDING_MODE;
    int c1cs = 0;
    // 0x8016A2C8: addiu       $sp, $sp, -0xA8
    ctx->r29 = ADD32(ctx->r29, -0XA8);
    // 0x8016A2CC: sw          $s1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r17;
    // 0x8016A2D0: lui         $s1, 0x801F
    ctx->r17 = S32(0X801F << 16);
    // 0x8016A2D4: sw          $s0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r16;
    // 0x8016A2D8: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016A2DC: addiu       $s1, $s1, -0x990
    ctx->r17 = ADD32(ctx->r17, -0X990);
    // 0x8016A2E0: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8016A2E4: lw          $v0, 0x3CB0($s1)
    ctx->r2 = MEM_W(0X3CB0, ctx->r17);
    // 0x8016A2E8: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(0X0, ctx->r16);
    // 0x8016A2EC: addiu       $at, $zero, -0x4
    ctx->r1 = ADD32(0, -0X4);
    // 0x8016A2F0: beq         $v0, $at, L_8016A304
    if (ctx->r2 == ctx->r1) {
        // 0x8016A2F4: sw          $t6, 0xA0($sp)
        MEM_W(0XA0, ctx->r29) = ctx->r14;
        goto L_8016A304;
    }
    // 0x8016A2F4: sw          $t6, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r14;
    // 0x8016A2F8: addiu       $at, $zero, -0x63
    ctx->r1 = ADD32(0, -0X63);
    // 0x8016A2FC: bnel        $v0, $at, L_8016A358
    if (ctx->r2 != ctx->r1) {
        // 0x8016A300: sb          $zero, 0x3CA7($s1)
        MEM_B(0X3CA7, ctx->r17) = 0;
        goto L_8016A358;
    }
    goto skip_0;
    // 0x8016A300: sb          $zero, 0x3CA7($s1)
    MEM_B(0X3CA7, ctx->r17) = 0;
    skip_0:
L_8016A304:
    // 0x8016A304: lbu         $v1, 0x100E($s1)
    ctx->r3 = MEM_BU(0X100E, ctx->r17);
    // 0x8016A308: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x8016A30C: addiu       $at, $zero, -0x63
    ctx->r1 = ADD32(0, -0X63);
    // 0x8016A310: andi        $t7, $v1, 0x80
    ctx->r15 = ctx->r3 & 0X80;
    // 0x8016A314: beq         $t7, $zero, L_8016A340
    if (ctx->r15 == 0) {
        // 0x8016A318: andi        $t8, $v1, 0x7F
        ctx->r24 = ctx->r3 & 0X7F;
        goto L_8016A340;
    }
    // 0x8016A318: andi        $t8, $v1, 0x7F
    ctx->r24 = ctx->r3 & 0X7F;
    // 0x8016A31C: sb          $t8, 0x100E($s1)
    MEM_B(0X100E, ctx->r17) = ctx->r24;
    // 0x8016A320: sb          $zero, 0x9B($s0)
    MEM_B(0X9B, ctx->r16) = 0;
    // 0x8016A324: lui         $t9, 0x8081
    ctx->r25 = S32(0X8081 << 16);
    // 0x8016A328: addiu       $t9, $t9, 0x5820
    ctx->r25 = ADD32(ctx->r25, 0X5820);
    // 0x8016A32C: addiu       $t1, $zero, 0x248
    ctx->r9 = ADD32(0, 0X248);
    // 0x8016A330: sw          $t1, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r9;
    // 0x8016A334: sw          $t9, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r25;
    // 0x8016A338: b           L_8016ABF8
    // 0x8016A33C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_8016ABF8;
    // 0x8016A33C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_8016A340:
    // 0x8016A340: bne         $v0, $at, L_8016A358
    if (ctx->r2 != ctx->r1) {
        // 0x8016A344: sb          $t2, 0x3CA7($s1)
        MEM_B(0X3CA7, ctx->r17) = ctx->r10;
        goto L_8016A358;
    }
    // 0x8016A344: sb          $t2, 0x3CA7($s1)
    MEM_B(0X3CA7, ctx->r17) = ctx->r10;
    // 0x8016A348: addiu       $t3, $zero, 0x2
    ctx->r11 = ADD32(0, 0X2);
    // 0x8016A34C: b           L_8016A358
    // 0x8016A350: sw          $t3, 0x3CB0($s1)
    MEM_W(0X3CB0, ctx->r17) = ctx->r11;
    goto L_8016A358;
    // 0x8016A350: sw          $t3, 0x3CB0($s1)
    MEM_W(0X3CB0, ctx->r17) = ctx->r11;
    // 0x8016A354: sb          $zero, 0x3CA7($s1)
    MEM_B(0X3CA7, ctx->r17) = 0;
L_8016A358:
    // 0x8016A358: lw          $v1, 0x0($s1)
    ctx->r3 = MEM_W(0X0, ctx->r17);
    // 0x8016A35C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016A360: bnel        $v1, $at, L_8016A390
    if (ctx->r3 != ctx->r1) {
        // 0x8016A364: lhu         $v0, 0x3F4A($s1)
        ctx->r2 = MEM_HU(0X3F4A, ctx->r17);
        goto L_8016A390;
    }
    goto skip_1;
    // 0x8016A364: lhu         $v0, 0x3F4A($s1)
    ctx->r2 = MEM_HU(0X3F4A, ctx->r17);
    skip_1:
    // 0x8016A368: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
    // 0x8016A36C: sb          $zero, 0x9B($s0)
    MEM_B(0X9B, ctx->r16) = 0;
    // 0x8016A370: lui         $t4, 0x8080
    ctx->r12 = S32(0X8080 << 16);
    // 0x8016A374: addiu       $t4, $t4, 0x3F30
    ctx->r12 = ADD32(ctx->r12, 0X3F30);
    // 0x8016A378: addiu       $t5, $zero, 0x210
    ctx->r13 = ADD32(0, 0X210);
    // 0x8016A37C: sw          $t5, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r13;
    // 0x8016A380: sw          $t4, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r12;
    // 0x8016A384: b           L_8016ABF8
    // 0x8016A388: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    goto L_8016ABF8;
    // 0x8016A388: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
    // 0x8016A38C: lhu         $v0, 0x3F4A($s1)
    ctx->r2 = MEM_HU(0X3F4A, ctx->r17);
L_8016A390:
    // 0x8016A390: ori         $at, $zero, 0xFFEF
    ctx->r1 = 0 | 0XFFEF;
    // 0x8016A394: beq         $v0, $at, L_8016A3A4
    if (ctx->r2 == ctx->r1) {
        // 0x8016A398: ori         $at, $zero, 0xFFF0
        ctx->r1 = 0 | 0XFFF0;
        goto L_8016A3A4;
    }
    // 0x8016A398: ori         $at, $zero, 0xFFF0
    ctx->r1 = 0 | 0XFFF0;
    // 0x8016A39C: bnel        $v0, $at, L_8016A4E8
    if (ctx->r2 != ctx->r1) {
        // 0x8016A3A0: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
        goto L_8016A4E8;
    }
    goto skip_2;
    // 0x8016A3A0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    skip_2:
L_8016A3A4:
    // 0x8016A3A4: lbu         $t6, 0xF19($s1)
    ctx->r14 = MEM_BU(0XF19, ctx->r17);
    // 0x8016A3A8: sra         $a1, $v1, 4
    ctx->r5 = S32(ctx->r3) >> 4;
    // 0x8016A3AC: sra         $a0, $v1, 9
    ctx->r4 = S32(ctx->r3) >> 9;
    // 0x8016A3B0: andi        $t7, $t6, 0x80
    ctx->r15 = ctx->r14 & 0X80;
    // 0x8016A3B4: beq         $t7, $zero, L_8016A42C
    if (ctx->r15 == 0) {
        // 0x8016A3B8: andi        $a1, $a1, 0x1F
        ctx->r5 = ctx->r5 & 0X1F;
        goto L_8016A42C;
    }
    // 0x8016A3B8: andi        $a1, $a1, 0x1F
    ctx->r5 = ctx->r5 & 0X1F;
    // 0x8016A3BC: addiu       $at, $zero, 0x4D
    ctx->r1 = ADD32(0, 0X4D);
    // 0x8016A3C0: bnel        $a0, $at, L_8016A3D4
    if (ctx->r4 != ctx->r1) {
        // 0x8016A3C4: addiu       $at, $zero, 0x4A
        ctx->r1 = ADD32(0, 0X4A);
        goto L_8016A3D4;
    }
    goto skip_3;
    // 0x8016A3C4: addiu       $at, $zero, 0x4A
    ctx->r1 = ADD32(0, 0X4A);
    skip_3:
    // 0x8016A3C8: b           L_8016A42C
    // 0x8016A3CC: addiu       $a0, $zero, 0x57
    ctx->r4 = ADD32(0, 0X57);
    goto L_8016A42C;
    // 0x8016A3CC: addiu       $a0, $zero, 0x57
    ctx->r4 = ADD32(0, 0X57);
    // 0x8016A3D0: addiu       $at, $zero, 0x4A
    ctx->r1 = ADD32(0, 0X4A);
L_8016A3D4:
    // 0x8016A3D4: bnel        $a0, $at, L_8016A3E8
    if (ctx->r4 != ctx->r1) {
        // 0x8016A3D8: addiu       $at, $zero, 0x5A
        ctx->r1 = ADD32(0, 0X5A);
        goto L_8016A3E8;
    }
    goto skip_4;
    // 0x8016A3D8: addiu       $at, $zero, 0x5A
    ctx->r1 = ADD32(0, 0X5A);
    skip_4:
    // 0x8016A3DC: b           L_8016A42C
    // 0x8016A3E0: addiu       $a0, $zero, 0x45
    ctx->r4 = ADD32(0, 0X45);
    goto L_8016A42C;
    // 0x8016A3E0: addiu       $a0, $zero, 0x45
    ctx->r4 = ADD32(0, 0X45);
    // 0x8016A3E4: addiu       $at, $zero, 0x5A
    ctx->r1 = ADD32(0, 0X5A);
L_8016A3E8:
    // 0x8016A3E8: bnel        $a0, $at, L_8016A3FC
    if (ctx->r4 != ctx->r1) {
        // 0x8016A3EC: addiu       $at, $zero, 0x59
        ctx->r1 = ADD32(0, 0X59);
        goto L_8016A3FC;
    }
    goto skip_5;
    // 0x8016A3EC: addiu       $at, $zero, 0x59
    ctx->r1 = ADD32(0, 0X59);
    skip_5:
    // 0x8016A3F0: b           L_8016A42C
    // 0x8016A3F4: addiu       $a0, $zero, 0x5B
    ctx->r4 = ADD32(0, 0X5B);
    goto L_8016A42C;
    // 0x8016A3F4: addiu       $a0, $zero, 0x5B
    ctx->r4 = ADD32(0, 0X5B);
    // 0x8016A3F8: addiu       $at, $zero, 0x59
    ctx->r1 = ADD32(0, 0X59);
L_8016A3FC:
    // 0x8016A3FC: beq         $a0, $at, L_8016A428
    if (ctx->r4 == ctx->r1) {
        // 0x8016A400: ori         $t8, $zero, 0xFFF0
        ctx->r24 = 0 | 0XFFF0;
        goto L_8016A428;
    }
    // 0x8016A400: ori         $t8, $zero, 0xFFF0
    ctx->r24 = 0 | 0XFFF0;
    // 0x8016A404: addiu       $at, $zero, 0x58
    ctx->r1 = ADD32(0, 0X58);
    // 0x8016A408: beq         $a0, $at, L_8016A428
    if (ctx->r4 == ctx->r1) {
        // 0x8016A40C: addiu       $at, $zero, 0x19
        ctx->r1 = ADD32(0, 0X19);
        goto L_8016A428;
    }
    // 0x8016A40C: addiu       $at, $zero, 0x19
    ctx->r1 = ADD32(0, 0X19);
    // 0x8016A410: beq         $a0, $at, L_8016A428
    if (ctx->r4 == ctx->r1) {
        // 0x8016A414: addiu       $at, $zero, 0x2F
        ctx->r1 = ADD32(0, 0X2F);
        goto L_8016A428;
    }
    // 0x8016A414: addiu       $at, $zero, 0x2F
    ctx->r1 = ADD32(0, 0X2F);
    // 0x8016A418: beq         $a0, $at, L_8016A428
    if (ctx->r4 == ctx->r1) {
        // 0x8016A41C: addiu       $at, $zero, 0x68
        ctx->r1 = ADD32(0, 0X68);
        goto L_8016A428;
    }
    // 0x8016A41C: addiu       $at, $zero, 0x68
    ctx->r1 = ADD32(0, 0X68);
    // 0x8016A420: bnel        $a0, $at, L_8016A430
    if (ctx->r4 != ctx->r1) {
        // 0x8016A424: lbu         $t9, 0xF0C($s1)
        ctx->r25 = MEM_BU(0XF0C, ctx->r17);
        goto L_8016A430;
    }
    goto skip_6;
    // 0x8016A424: lbu         $t9, 0xF0C($s1)
    ctx->r25 = MEM_BU(0XF0C, ctx->r17);
    skip_6:
L_8016A428:
    // 0x8016A428: sh          $t8, 0x3F4A($s1)
    MEM_H(0X3F4A, ctx->r17) = ctx->r24;
L_8016A42C:
    // 0x8016A42C: lbu         $t9, 0xF0C($s1)
    ctx->r25 = MEM_BU(0XF0C, ctx->r17);
L_8016A430:
    // 0x8016A430: addiu       $at, $zero, 0x42
    ctx->r1 = ADD32(0, 0X42);
    // 0x8016A434: andi        $t1, $t9, 0x2
    ctx->r9 = ctx->r25 & 0X2;
    // 0x8016A438: beq         $t1, $zero, L_8016A460
    if (ctx->r9 == 0) {
        // 0x8016A43C: lui         $t9, 0x801C
        ctx->r25 = S32(0X801C << 16);
        goto L_8016A460;
    }
    // 0x8016A43C: lui         $t9, 0x801C
    ctx->r25 = S32(0X801C << 16);
    // 0x8016A440: bnel        $a0, $at, L_8016A454
    if (ctx->r4 != ctx->r1) {
        // 0x8016A444: addiu       $at, $zero, 0x43
        ctx->r1 = ADD32(0, 0X43);
        goto L_8016A454;
    }
    goto skip_7;
    // 0x8016A444: addiu       $at, $zero, 0x43
    ctx->r1 = ADD32(0, 0X43);
    skip_7:
    // 0x8016A448: b           L_8016A460
    // 0x8016A44C: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    goto L_8016A460;
    // 0x8016A44C: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    // 0x8016A450: addiu       $at, $zero, 0x43
    ctx->r1 = ADD32(0, 0X43);
L_8016A454:
    // 0x8016A454: bne         $a0, $at, L_8016A460
    if (ctx->r4 != ctx->r1) {
        // 0x8016A458: ori         $t2, $zero, 0xFFF1
        ctx->r10 = 0 | 0XFFF1;
        goto L_8016A460;
    }
    // 0x8016A458: ori         $t2, $zero, 0xFFF1
    ctx->r10 = 0 | 0XFFF1;
    // 0x8016A45C: sh          $t2, 0x3F4A($s1)
    MEM_H(0X3F4A, ctx->r17) = ctx->r10;
L_8016A460:
    // 0x8016A460: lbu         $t3, 0xF2C($s1)
    ctx->r11 = MEM_BU(0XF2C, ctx->r17);
    // 0x8016A464: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x8016A468: andi        $t4, $t3, 0x20
    ctx->r12 = ctx->r11 & 0X20;
    // 0x8016A46C: beql        $t4, $zero, L_8016A484
    if (ctx->r12 == 0) {
        // 0x8016A470: lbu         $t6, 0xF2F($s1)
        ctx->r14 = MEM_BU(0XF2F, ctx->r17);
        goto L_8016A484;
    }
    goto skip_8;
    // 0x8016A470: lbu         $t6, 0xF2F($s1)
    ctx->r14 = MEM_BU(0XF2F, ctx->r17);
    skip_8:
    // 0x8016A474: bne         $a0, $at, L_8016A480
    if (ctx->r4 != ctx->r1) {
        // 0x8016A478: ori         $t5, $zero, 0xFFF2
        ctx->r13 = 0 | 0XFFF2;
        goto L_8016A480;
    }
    // 0x8016A478: ori         $t5, $zero, 0xFFF2
    ctx->r13 = 0 | 0XFFF2;
    // 0x8016A47C: sh          $t5, 0x3F4A($s1)
    MEM_H(0X3F4A, ctx->r17) = ctx->r13;
L_8016A480:
    // 0x8016A480: lbu         $t6, 0xF2F($s1)
    ctx->r14 = MEM_BU(0XF2F, ctx->r17);
L_8016A484:
    // 0x8016A484: addiu       $at, $zero, 0x34
    ctx->r1 = ADD32(0, 0X34);
    // 0x8016A488: andi        $t7, $t6, 0x80
    ctx->r15 = ctx->r14 & 0X80;
    // 0x8016A48C: beq         $t7, $zero, L_8016A4AC
    if (ctx->r15 == 0) {
        // 0x8016A490: nop
    
        goto L_8016A4AC;
    }
    // 0x8016A490: nop

    // 0x8016A494: beq         $a0, $at, L_8016A4A8
    if (ctx->r4 == ctx->r1) {
        // 0x8016A498: ori         $t8, $zero, 0xFFF0
        ctx->r24 = 0 | 0XFFF0;
        goto L_8016A4A8;
    }
    // 0x8016A498: ori         $t8, $zero, 0xFFF0
    ctx->r24 = 0 | 0XFFF0;
    // 0x8016A49C: addiu       $at, $zero, 0x35
    ctx->r1 = ADD32(0, 0X35);
    // 0x8016A4A0: bne         $a0, $at, L_8016A4AC
    if (ctx->r4 != ctx->r1) {
        // 0x8016A4A4: nop
    
        goto L_8016A4AC;
    }
    // 0x8016A4A4: nop

L_8016A4A8:
    // 0x8016A4A8: sh          $t8, 0x3F4A($s1)
    MEM_H(0X3F4A, ctx->r17) = ctx->r24;
L_8016A4AC:
    // 0x8016A4AC: lbu         $t9, 0x2078($t9)
    ctx->r25 = MEM_BU(0X2078, ctx->r25);
    // 0x8016A4B0: addiu       $at, $zero, 0x2A
    ctx->r1 = ADD32(0, 0X2A);
    // 0x8016A4B4: addu        $t1, $s1, $t9
    ctx->r9 = ADD32(ctx->r17, ctx->r25);
    // 0x8016A4B8: lbu         $t2, 0x70($t1)
    ctx->r10 = MEM_BU(0X70, ctx->r9);
    // 0x8016A4BC: beq         $t2, $zero, L_8016A4D8
    if (ctx->r10 == 0) {
        // 0x8016A4C0: nop
    
        goto L_8016A4D8;
    }
    // 0x8016A4C0: nop

    // 0x8016A4C4: bne         $a0, $at, L_8016A4D8
    if (ctx->r4 != ctx->r1) {
        // 0x8016A4C8: addiu       $at, $zero, 0x54A0
        ctx->r1 = ADD32(0, 0X54A0);
        goto L_8016A4D8;
    }
    // 0x8016A4C8: addiu       $at, $zero, 0x54A0
    ctx->r1 = ADD32(0, 0X54A0);
    // 0x8016A4CC: beq         $v1, $at, L_8016A4D8
    if (ctx->r3 == ctx->r1) {
        // 0x8016A4D0: ori         $t3, $zero, 0xFFF4
        ctx->r11 = 0 | 0XFFF4;
        goto L_8016A4D8;
    }
    // 0x8016A4D0: ori         $t3, $zero, 0xFFF4
    ctx->r11 = 0 | 0XFFF4;
    // 0x8016A4D4: sh          $t3, 0x3F4A($s1)
    //MEM_H(0X3F4A, ctx->r17) = ctx->r11;
L_8016A4D8:
    // 0x8016A4D8: jal         0x80130768
    // 0x8016A4DC: andi        $a2, $v1, 0xF
    ctx->r6 = ctx->r3 & 0XF;
    Entrance_Create(rdram, ctx);
    goto after_0;
    // 0x8016A4DC: andi        $a2, $v1, 0xF
    ctx->r6 = ctx->r3 & 0XF;
    after_0:
    // 0x8016A4E0: sw          $v0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r2;
    // 0x8016A4E4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
L_8016A4E8:
    // 0x8016A4E8: jal         0x80173880
    // 0x8016A4EC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    GameState_Realloc(rdram, ctx);
    goto after_1;
    // 0x8016A4EC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_1:
    // 0x8016A4F0: jal         0x80163804
    // 0x8016A4F4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    KaleidoManager_Init(rdram, ctx);
    goto after_2;
    // 0x8016A4F4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x8016A4F8: jal         0x80160CA0
    // 0x8016A4FC: nop

    ShrinkWindow_Init(rdram, ctx);
    goto after_3;
    // 0x8016A4FC: nop

    after_3:
    // 0x8016A500: addiu       $a0, $s0, 0xB8
    ctx->r4 = ADD32(ctx->r16, 0XB8);
    // 0x8016A504: sw          $a0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r4;
    // 0x8016A508: jal         0x8013EEF4
    // 0x8016A50C: lw          $a1, 0xA0($sp)
    ctx->r5 = MEM_W(0XA0, ctx->r29);
    View_Init(rdram, ctx);
    goto after_4;
    // 0x8016A50C: lw          $a1, 0xA0($sp)
    ctx->r5 = MEM_W(0XA0, ctx->r29);
    after_4:
    // 0x8016A510: jal         0x801A3EC0
    // 0x8016A514: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    Audio_SetExtraFilter(rdram, ctx);
    goto after_5;
    // 0x8016A514: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_5:
    // 0x8016A518: jal         0x8012AA48
    // 0x8016A51C: nop

    Quake_Init(rdram, ctx);
    goto after_6;
    // 0x8016A51C: nop

    after_6:
    // 0x8016A520: jal         0x8012AE68
    // 0x8016A524: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Distortion_Init(rdram, ctx);
    goto after_7;
    // 0x8016A524: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_7:
    // 0x8016A528: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016A52C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_8016A530:
    // 0x8016A530: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x8016A534: slti        $at, $a1, 0x4
    ctx->r1 = SIGNED(ctx->r5) < 0X4 ? 1 : 0;
    // 0x8016A538: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016A53C: bne         $at, $zero, L_8016A530
    if (ctx->r1 != 0) {
        // 0x8016A540: sw          $zero, 0x7FC($v0)
        MEM_W(0X7FC, ctx->r2) = 0;
        goto L_8016A530;
    }
    // 0x8016A540: sw          $zero, 0x7FC($v0)
    MEM_W(0X7FC, ctx->r2) = 0;
    // 0x8016A544: addiu       $a0, $s0, 0x220
    ctx->r4 = ADD32(ctx->r16, 0X220);
    // 0x8016A548: addiu       $a2, $s0, 0x830
    ctx->r6 = ADD32(ctx->r16, 0X830);
    // 0x8016A54C: sw          $a2, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r6;
    // 0x8016A550: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8016A554: lw          $a1, 0x44($sp)
    ctx->r5 = MEM_W(0X44, ctx->r29);
    // 0x8016A558: jal         0x800DDDD0
    // 0x8016A55C: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    Camera_Init(rdram, ctx);
    goto after_8;
    // 0x8016A55C: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    after_8:
    // 0x8016A560: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(0X40, ctx->r29);
    // 0x8016A564: jal         0x800DE308
    // 0x8016A568: addiu       $a1, $zero, 0x7
    ctx->r5 = ADD32(0, 0X7);
    Camera_ChangeStatus(rdram, ctx);
    goto after_9;
    // 0x8016A568: addiu       $a1, $zero, 0x7
    ctx->r5 = ADD32(0, 0X7);
    after_9:
    // 0x8016A56C: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x8016A570: addiu       $a0, $s0, 0x398
    ctx->r4 = ADD32(ctx->r16, 0X398);
L_8016A574:
    // 0x8016A574: lw          $a1, 0x44($sp)
    ctx->r5 = MEM_W(0X44, ctx->r29);
    // 0x8016A578: lw          $a2, 0x3C($sp)
    ctx->r6 = MEM_W(0X3C, ctx->r29);
    // 0x8016A57C: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    // 0x8016A580: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x8016A584: jal         0x800DDDD0
    // 0x8016A588: sw          $t0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r8;
    Camera_Init(rdram, ctx);
    goto after_10;
    // 0x8016A588: sw          $t0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r8;
    after_10:
    // 0x8016A58C: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(0X48, ctx->r29);
    // 0x8016A590: jal         0x800DE308
    // 0x8016A594: addiu       $a1, $zero, 0x100
    ctx->r5 = ADD32(0, 0X100);
    Camera_ChangeStatus(rdram, ctx);
    goto after_11;
    // 0x8016A594: addiu       $a1, $zero, 0x100
    ctx->r5 = ADD32(0, 0X100);
    after_11:
    // 0x8016A598: lw          $t0, 0x4C($sp)
    ctx->r8 = MEM_W(0X4C, ctx->r29);
    // 0x8016A59C: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(0X48, ctx->r29);
    // 0x8016A5A0: addiu       $at, $zero, 0x468
    ctx->r1 = ADD32(0, 0X468);
    // 0x8016A5A4: addiu       $t0, $t0, 0x178
    ctx->r8 = ADD32(ctx->r8, 0X178);
    // 0x8016A5A8: bne         $t0, $at, L_8016A574
    if (ctx->r8 != ctx->r1) {
        // 0x8016A5AC: addiu       $a0, $a0, 0x178
        ctx->r4 = ADD32(ctx->r4, 0X178);
        goto L_8016A574;
    }
    // 0x8016A5AC: addiu       $a0, $a0, 0x178
    ctx->r4 = ADD32(ctx->r4, 0X178);
    // 0x8016A5B0: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(0X40, ctx->r29);
    // 0x8016A5B4: addiu       $a1, $zero, 0x7F
    ctx->r5 = ADD32(0, 0X7F);
    // 0x8016A5B8: sw          $a0, 0x800($s0)
    MEM_W(0X800, ctx->r16) = ctx->r4;
    // 0x8016A5BC: sh          $zero, 0x130($a0)
    MEM_H(0X130, ctx->r4) = 0;
    // 0x8016A5C0: jal         0x800DFF18
    // 0x8016A5C4: sh          $zero, 0x810($s0)
    MEM_H(0X810, ctx->r16) = 0;
    Camera_OverwriteStateFlags(rdram, ctx);
    goto after_12;
    // 0x8016A5C4: sh          $zero, 0x810($s0)
    MEM_H(0X810, ctx->r16) = 0;
    after_12:
    // 0x8016A5C8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016A5CC: jal         0x80146E70
    // 0x8016A5D0: addiu       $a1, $s0, 0x46B8
    ctx->r5 = ADD32(ctx->r16, 0X46B8);
    Sram_Alloc(rdram, ctx);
    goto after_13;
    // 0x8016A5D0: addiu       $a1, $s0, 0x46B8
    ctx->r5 = ADD32(ctx->r16, 0X46B8);
    after_13:
    // 0x8016A5D4: jal         0x801AAAA0
    // 0x8016A5D8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Regs_InitData(rdram, ctx);
    goto after_14;
    // 0x8016A5D8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_14:
    // 0x8016A5DC: jal         0x801586D4
    // 0x8016A5E0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Message_Init(rdram, ctx);
    goto after_15;
    // 0x8016A5E0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_15:
    // 0x8016A5E4: jal         0x801AA610
    // 0x8016A5E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    GameOver_Init(rdram, ctx);
    goto after_16;
    // 0x8016A5E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_16:
    // 0x8016A5EC: jal         0x800F0390
    // 0x8016A5F0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    SoundSource_InitAll(rdram, ctx);
    goto after_17;
    // 0x8016A5F0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_17:
    // 0x8016A5F4: jal         0x800EFE60
    // 0x8016A5F8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    EffFootmark_Init(rdram, ctx);
    goto after_18;
    // 0x8016A5F8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_18:
    // 0x8016A5FC: jal         0x800AF890
    // 0x8016A600: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Effect_Init(rdram, ctx);
    goto after_19;
    // 0x8016A600: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_19:
    // 0x8016A604: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016A608: jal         0x800B0050
    // 0x8016A60C: addiu       $a1, $zero, 0x64
    ctx->r5 = ADD32(0, 0X64);
    EffectSS_Init(rdram, ctx);
    goto after_20;
    // 0x8016A60C: addiu       $a1, $zero, 0x64
    ctx->r5 = ADD32(0, 0X64);
    after_20:
    // 0x8016A610: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016A614: ori         $at, $at, 0x8884
    ctx->r1 = ctx->r1 | 0X8884;
    // 0x8016A618: addu        $a1, $s0, $at
    ctx->r5 = ADD32(ctx->r16, ctx->r1);
    // 0x8016A61C: jal         0x800E2450
    // 0x8016A620: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    CollisionCheck_InitContext(rdram, ctx);
    goto after_21;
    // 0x8016A620: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_21:
    // 0x8016A624: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016A628: ori         $at, $at, 0x7104
    ctx->r1 = ctx->r1 | 0X7104;
    // 0x8016A62C: addu        $a0, $s0, $at
    ctx->r4 = ADD32(ctx->r16, ctx->r1);
    // 0x8016A630: jal         0x801358C8
    // 0x8016A634: sw          $a0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r4;
    AnimationContext_Reset(rdram, ctx);
    goto after_22;
    // 0x8016A634: sw          $a0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r4;
    after_22:
    // 0x8016A638: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016A63C: jal         0x800EA060
    // 0x8016A640: addiu       $a1, $s0, 0x1F24
    ctx->r5 = ADD32(ctx->r16, 0X1F24);
    Cutscene_InitContext(rdram, ctx);
    goto after_23;
    // 0x8016A640: addiu       $a1, $s0, 0x1F24
    ctx->r5 = ADD32(ctx->r16, 0X1F24);
    after_23:
    // 0x8016A644: lhu         $v0, 0x3F4A($s1)
    ctx->r2 = MEM_HU(0X3F4A, ctx->r17);
    // 0x8016A648: ori         $at, $zero, 0xFFEF
    ctx->r1 = 0 | 0XFFEF;
    // 0x8016A64C: beq         $v0, $at, L_8016A65C
    if (ctx->r2 == ctx->r1) {
        // 0x8016A650: ori         $t4, $zero, 0xFFEF
        ctx->r12 = 0 | 0XFFEF;
        goto L_8016A65C;
    }
    // 0x8016A650: ori         $t4, $zero, 0xFFEF
    ctx->r12 = 0 | 0XFFEF;
    // 0x8016A654: sw          $v0, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->r2;
    // 0x8016A658: sh          $t4, 0x3F4A($s1)
    MEM_H(0X3F4A, ctx->r17) = ctx->r12;
L_8016A65C:
    // 0x8016A65C: lw          $t5, 0x8($s1)
    ctx->r13 = MEM_W(0X8, ctx->r17);
    // 0x8016A660: ori         $at, $zero, 0xFFFD
    ctx->r1 = 0 | 0XFFFD;
    // 0x8016A664: bnel        $t5, $at, L_8016A674
    if (ctx->r13 != ctx->r1) {
        // 0x8016A668: lhu         $v0, 0x3F4E($s1)
        ctx->r2 = MEM_HU(0X3F4E, ctx->r17);
        goto L_8016A674;
    }
    goto skip_9;
    // 0x8016A668: lhu         $v0, 0x3F4E($s1)
    ctx->r2 = MEM_HU(0X3F4E, ctx->r17);
    skip_9:
    // 0x8016A66C: sw          $zero, 0x8($s1)
    MEM_W(0X8, ctx->r17) = 0;
    // 0x8016A670: lhu         $v0, 0x3F4E($s1)
    ctx->r2 = MEM_HU(0X3F4E, ctx->r17);
L_8016A674:
    // 0x8016A674: ori         $at, $zero, 0xFFFF
    ctx->r1 = 0 | 0XFFFF;
    // 0x8016A678: beql        $v0, $at, L_8016A68C
    if (ctx->r2 == ctx->r1) {
        // 0x8016A67C: lhu         $v0, 0xC($s1)
        ctx->r2 = MEM_HU(0XC, ctx->r17);
        goto L_8016A68C;
    }
    goto skip_10;
    // 0x8016A67C: lhu         $v0, 0xC($s1)
    ctx->r2 = MEM_HU(0XC, ctx->r17);
    skip_10:
    // 0x8016A680: sh          $v0, 0xC($s1)
    MEM_H(0XC, ctx->r17) = ctx->r2;
    // 0x8016A684: sh          $v0, 0x3F52($s1)
    MEM_H(0X3F52, ctx->r17) = ctx->r2;
    // 0x8016A688: lhu         $v0, 0xC($s1)
    ctx->r2 = MEM_HU(0XC, ctx->r17);
L_8016A68C:
    // 0x8016A68C: ori         $at, $zero, 0xC000
    ctx->r1 = 0 | 0XC000;
    // 0x8016A690: slt         $at, $v0, $at
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x8016A694: beq         $at, $zero, L_8016A6A0
    if (ctx->r1 == 0) {
        // 0x8016A698: slti        $at, $v0, 0x4555
        ctx->r1 = SIGNED(ctx->r2) < 0X4555 ? 1 : 0;
        goto L_8016A6A0;
    }
    // 0x8016A698: slti        $at, $v0, 0x4555
    ctx->r1 = SIGNED(ctx->r2) < 0X4555 ? 1 : 0;
    // 0x8016A69C: beq         $at, $zero, L_8016A6AC
    if (ctx->r1 == 0) {
        // 0x8016A6A0: addiu       $t6, $zero, 0x1
        ctx->r14 = ADD32(0, 0X1);
        goto L_8016A6AC;
    }
L_8016A6A0:
    // 0x8016A6A0: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x8016A6A4: b           L_8016A6B0
    // 0x8016A6A8: sw          $t6, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r14;
    goto L_8016A6B0;
    // 0x8016A6A8: sw          $t6, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r14;
L_8016A6AC:
    // 0x8016A6AC: sw          $zero, 0x10($s1)
    MEM_W(0X10, ctx->r17) = 0;
L_8016A6B0:
    // 0x8016A6B0: jal         0x800EDDB0
    // 0x8016A6B4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_800EDDB0(rdram, ctx);
    goto after_24;
    // 0x8016A6B4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_24:
    // 0x8016A6B8: lw          $v0, 0x3CA8($s1)
    ctx->r2 = MEM_W(0X3CA8, ctx->r17);
    // 0x8016A6BC: beq         $v0, $zero, L_8016A6CC
    if (ctx->r2 == 0) {
        // 0x8016A6C0: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
        goto L_8016A6CC;
    }
    // 0x8016A6C0: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8016A6C4: bnel        $v0, $at, L_8016A6E4
    if (ctx->r2 != ctx->r1) {
        // 0x8016A6C8: sh          $zero, 0x3DC0($s1)
        MEM_H(0X3DC0, ctx->r17) = 0;
        goto L_8016A6E4;
    }
    goto skip_11;
    // 0x8016A6C8: sh          $zero, 0x3DC0($s1)
    MEM_H(0X3DC0, ctx->r17) = 0;
    skip_11:
L_8016A6CC:
    // 0x8016A6CC: lw          $t7, 0x8($s1)
    ctx->r15 = MEM_W(0X8, ctx->r17);
    // 0x8016A6D0: ori         $at, $zero, 0xFFF0
    ctx->r1 = 0 | 0XFFF0;
    // 0x8016A6D4: slt         $at, $t7, $at
    ctx->r1 = SIGNED(ctx->r15) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x8016A6D8: bnel        $at, $zero, L_8016A708
    if (ctx->r1 != 0) {
        // 0x8016A6DC: sw          $zero, 0x3CAC($s1)
        MEM_W(0X3CAC, ctx->r17) = 0;
        goto L_8016A708;
    }
    goto skip_12;
    // 0x8016A6DC: sw          $zero, 0x3CAC($s1)
    MEM_W(0X3CAC, ctx->r17) = 0;
    skip_12:
    // 0x8016A6E0: sh          $zero, 0x3DC0($s1)
    MEM_H(0X3DC0, ctx->r17) = 0;
L_8016A6E4:
    // 0x8016A6E4: jal         0x80115D5C
    // 0x8016A6E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Magic_Reset(rdram, ctx);
    goto after_25;
    // 0x8016A6E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_25:
    // 0x8016A6EC: lw          $t8, 0x8($s1)
    ctx->r24 = MEM_W(0X8, ctx->r17);
    // 0x8016A6F0: sw          $zero, 0x8($s1)
    MEM_W(0X8, ctx->r17) = 0;
    // 0x8016A6F4: andi        $t9, $t8, 0xF
    ctx->r25 = ctx->r24 & 0XF;
    // 0x8016A6F8: addiu       $t1, $t9, 0x1
    ctx->r9 = ADD32(ctx->r25, 0X1);
    // 0x8016A6FC: b           L_8016A708
    // 0x8016A700: sw          $t1, 0x3CAC($s1)
    MEM_W(0X3CAC, ctx->r17) = ctx->r9;
    goto L_8016A708;
    // 0x8016A700: sw          $t1, 0x3CAC($s1)
    MEM_W(0X3CAC, ctx->r17) = ctx->r9;
    // 0x8016A704: sw          $zero, 0x3CAC($s1)
    MEM_W(0X3CAC, ctx->r17) = 0;
L_8016A708:
    // 0x8016A708: lw          $v0, 0x3CAC($s1)
    ctx->r2 = MEM_W(0X3CAC, ctx->r17);
    // 0x8016A70C: lw          $v1, 0x0($s1)
    ctx->r3 = MEM_W(0X0, ctx->r17);
    // 0x8016A710: sb          $v0, 0x87($sp)
    MEM_B(0X87, ctx->r29) = ctx->r2;
    // 0x8016A714: addu        $a0, $v0, $v1
    ctx->r4 = ADD32(ctx->r2, ctx->r3);
    // 0x8016A718: jal         0x80132338
    // 0x8016A71C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    Entrance_GetSceneIdAbsolute(rdram, ctx);
    goto after_26;
    // 0x8016A71C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_26:
    // 0x8016A720: lw          $a2, 0x0($s1)
    ctx->r6 = MEM_W(0X0, ctx->r17);
    // 0x8016A724: lw          $a1, 0x3CAC($s1)
    ctx->r5 = MEM_W(0X3CAC, ctx->r17);
    // 0x8016A728: sw          $v0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r2;
    // 0x8016A72C: addu        $a0, $a1, $a2
    ctx->r4 = ADD32(ctx->r5, ctx->r6);
    // 0x8016A730: jal         0x80132374
    // 0x8016A734: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    Entrance_GetSpawnNum(rdram, ctx);
    goto after_27;
    // 0x8016A734: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_27:
    // 0x8016A738: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016A73C: lw          $a1, 0x60($sp)
    ctx->r5 = MEM_W(0X60, ctx->r29);
    // 0x8016A740: jal         0x801693D4
    // 0x8016A744: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    Play_SpawnScene(rdram, ctx);
    goto after_28;
    // 0x8016A744: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    after_28:
    // 0x8016A748: jal         0x801639EC
    // 0x8016A74C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    KaleidoScopeCall_Init(rdram, ctx);
    goto after_29;
    // 0x8016A74C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_29:
    // 0x8016A750: jal         0x80121FC4
    // 0x8016A754: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Interface_Init(rdram, ctx);
    goto after_30;
    // 0x8016A754: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_30:
    // 0x8016A758: lhu         $v0, 0x3F4E($s1)
    ctx->r2 = MEM_HU(0X3F4E, ctx->r17);
    // 0x8016A75C: ori         $at, $zero, 0xFFFF
    ctx->r1 = 0 | 0XFFFF;
    // 0x8016A760: beq         $v0, $at, L_8016A7A0
    if (ctx->r2 == ctx->r1) {
        // 0x8016A764: ori         $at, $zero, 0x8000
        ctx->r1 = 0 | 0X8000;
        goto L_8016A7A0;
    }
    // 0x8016A764: ori         $at, $zero, 0x8000
    ctx->r1 = 0 | 0X8000;
    // 0x8016A768: bne         $v0, $at, L_8016A79C
    if (ctx->r2 != ctx->r1) {
        // 0x8016A76C: ori         $t8, $zero, 0xFFFD
        ctx->r24 = 0 | 0XFFFD;
        goto L_8016A79C;
    }
    // 0x8016A76C: ori         $t8, $zero, 0xFFFD
    ctx->r24 = 0 | 0XFFFD;
    // 0x8016A770: lw          $t2, 0x18($s1)
    ctx->r10 = MEM_W(0X18, ctx->r17);
    // 0x8016A774: lw          $t4, 0x1C($s1)
    ctx->r12 = MEM_W(0X1C, ctx->r17);
    // 0x8016A778: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x8016A77C: ori         $t7, $zero, 0xFFFE
    ctx->r15 = 0 | 0XFFFE;
    // 0x8016A780: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8016A784: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x8016A788: sw          $t3, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->r11;
    // 0x8016A78C: sw          $t5, 0x1C($s1)
    MEM_W(0X1C, ctx->r17) = ctx->r13;
    // 0x8016A790: sb          $t6, 0x3F54($s1)
    MEM_B(0X3F54, ctx->r17) = ctx->r14;
    // 0x8016A794: b           L_8016A7A0
    // 0x8016A798: sh          $t7, 0x3F4E($s1)
    MEM_H(0X3F4E, ctx->r17) = ctx->r15;
    goto L_8016A7A0;
    // 0x8016A798: sh          $t7, 0x3F4E($s1)
    MEM_H(0X3F4E, ctx->r17) = ctx->r15;
L_8016A79C:
    // 0x8016A79C: sh          $t8, 0x3F4E($s1)
    MEM_H(0X3F4E, ctx->r17) = ctx->r24;
L_8016A7A0:
    // 0x8016A7A0: jal         0x80165608
    // 0x8016A7A4: nop

    Play_InitMotionBlur(rdram, ctx);
    goto after_31;
    // 0x8016A7A4: nop

    after_31:
    // 0x8016A7A8: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x8016A7AC: addiu       $v0, $v0, 0x3F60
    ctx->r2 = ADD32(ctx->r2, 0X3F60);
    // 0x8016A7B0: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(0X0, ctx->r2);
    // 0x8016A7B4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016A7B8: ori         $at, $at, 0x8B4C
    ctx->r1 = ctx->r1 | 0X8B4C;
    // 0x8016A7BC: sh          $zero, 0x190($t9)
    MEM_H(0X190, ctx->r25) = 0;
    // 0x8016A7C0: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(0X0, ctx->r2);
    // 0x8016A7C4: addu        $a0, $s0, $at
    ctx->r4 = ADD32(ctx->r16, ctx->r1);
    // 0x8016A7C8: sh          $zero, 0x186($t1)
    MEM_H(0X186, ctx->r9) = 0;
    // 0x8016A7CC: jal         0x8016FD2C
    // 0x8016A7D0: sw          $a0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r4;
    PreRender_Init(rdram, ctx);
    goto after_32;
    // 0x8016A7D0: sw          $a0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r4;
    after_32:
    // 0x8016A7D4: lui         $a1, 0x8020
    ctx->r5 = S32(0X8020 << 16);
    // 0x8016A7D8: lui         $a2, 0x8020
    ctx->r6 = S32(0X8020 << 16);
    // 0x8016A7DC: lh          $a2, -0x4432($a2)
    ctx->r6 = MEM_H(-0X4432, ctx->r6);
    // 0x8016A7E0: lh          $a1, -0x4434($a1)
    ctx->r5 = MEM_H(-0X4434, ctx->r5);
    // 0x8016A7E4: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    // 0x8016A7E8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x8016A7EC: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x8016A7F0: jal         0x8016FCF0
    // 0x8016A7F4: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    PreRender_SetValuesSave(rdram, ctx);
    goto after_33;
    // 0x8016A7F4: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    after_33:
    // 0x8016A7F8: lui         $a1, 0x8020
    ctx->r5 = S32(0X8020 << 16);
    // 0x8016A7FC: lui         $a2, 0x8020
    ctx->r6 = S32(0X8020 << 16);
    // 0x8016A800: lh          $a2, -0x4432($a2)
    ctx->r6 = MEM_H(-0X4432, ctx->r6);
    // 0x8016A804: lh          $a1, -0x4434($a1)
    ctx->r5 = MEM_H(-0X4434, ctx->r5);
    // 0x8016A808: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    // 0x8016A80C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x8016A810: jal         0x8016FD60
    // 0x8016A814: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    PreRender_SetValues(rdram, ctx);
    goto after_34;
    // 0x8016A814: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_34:
    // 0x8016A818: lui         $t2, 0x8020
    ctx->r10 = S32(0X8020 << 16);
    // 0x8016A81C: lw          $t2, -0x4470($t2)
    ctx->r10 = MEM_W(-0X4470, ctx->r10);
    // 0x8016A820: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A824: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A828: sw          $t2, -0x719C($at)
    MEM_W(-0X719C, ctx->r1) = ctx->r10;
    // 0x8016A82C: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A830: lui         $t3, 0x8078
    ctx->r11 = S32(0X8078 << 16);
    // 0x8016A834: addiu       $t3, $t3, 0x0
    ctx->r11 = ADD32(ctx->r11, 0X0);
    // 0x8016A838: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A83C: sw          $t3, -0x71A4($at)
    MEM_W(-0X71A4, ctx->r1) = ctx->r11;
    // 0x8016A840: lui         $v0, 0x8078
    ctx->r2 = S32(0X8078 << 16);
    // 0x8016A844: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A848: addiu       $v0, $v0, 0x4600
    ctx->r2 = ADD32(ctx->r2, 0X4600);
    // 0x8016A84C: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A850: sw          $v0, -0x7198($at)
    MEM_W(-0X7198, ctx->r1) = ctx->r2;
    // 0x8016A854: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A858: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A85C: sw          $v0, -0x71A8($at)
    MEM_W(-0X71A8, ctx->r1) = ctx->r2;
    // 0x8016A860: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A864: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A868: sw          $v0, -0x71A0($at)
    MEM_W(-0X71A0, ctx->r1) = ctx->r2;
    // 0x8016A86C: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8016A870: sw          $zero, 0x6D10($at)
    MEM_W(0X6D10, ctx->r1) = 0;
    // 0x8016A874: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A878: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A87C: sb          $zero, -0x74B6($at)
    MEM_B(-0X74B6, ctx->r1) = 0;
    // 0x8016A880: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x8016A884: sb          $zero, 0xD54($at)
    MEM_B(0XD54, ctx->r1) = 0;
    // 0x8016A888: jal         0x80122660
    // 0x8016A88C: addiu       $a0, $s0, 0x828
    ctx->r4 = ADD32(ctx->r16, 0X828);
    FrameAdvance_Init(rdram, ctx);
    goto after_35;
    // 0x8016A88C: addiu       $a0, $s0, 0x828
    ctx->r4 = ADD32(ctx->r16, 0X828);
    after_35:
    // 0x8016A890: jal         0x80090420
    // 0x8016A894: nop

    osGetTime_recomp(rdram, ctx);
    goto after_36;
    // 0x8016A894: nop

    after_36:
    // 0x8016A898: jal         0x80086FD0
    // 0x8016A89C: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
    Rand_Seed(rdram, ctx);
    goto after_37;
    // 0x8016A89C: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
    after_37:
    // 0x8016A8A0: jal         0x80180160
    // 0x8016A8A4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Matrix_Init(rdram, ctx);
    goto after_38;
    // 0x8016A8A4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_38:
    // 0x8016A8A8: lui         $t4, 0x8017
    ctx->r12 = S32(0X8017 << 16);
    // 0x8016A8AC: lui         $t5, 0x8016
    ctx->r13 = S32(0X8016 << 16);
    // 0x8016A8B0: addiu       $t4, $t4, -0x709C
    ctx->r12 = ADD32(ctx->r12, -0X709C);
    // 0x8016A8B4: addiu       $t5, $t5, 0x613C
    ctx->r13 = ADD32(ctx->r13, 0X613C);
    // 0x8016A8B8: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A8BC: sw          $t4, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r12;
    // 0x8016A8C0: sw          $t5, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r13;
    // 0x8016A8C4: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A8C8: addiu       $t6, $zero, -0x14
    ctx->r14 = ADD32(0, -0X14);
    // 0x8016A8CC: sb          $t6, -0x778B($at)
    MEM_B(-0X778B, ctx->r1) = ctx->r14;
    // 0x8016A8D0: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A8D4: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A8D8: sh          $zero, -0x778A($at)
    MEM_H(-0X778A, ctx->r1) = 0;
    // 0x8016A8DC: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A8E0: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A8E4: sh          $zero, -0x7788($at)
    MEM_H(-0X7788, ctx->r1) = 0;
    // 0x8016A8E8: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A8EC: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A8F0: sb          $zero, -0x77BB($at)
    MEM_B(-0X77BB, ctx->r1) = 0;
    // 0x8016A8F4: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A8F8: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A8FC: sb          $zero, -0x77BC($at)
    MEM_B(-0X77BC, ctx->r1) = 0;
    // 0x8016A900: lw          $t7, 0x3CA8($s1)
    ctx->r15 = MEM_W(0X3CA8, ctx->r17);
    // 0x8016A904: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8016A908: beql        $t7, $at, L_8016A964
    if (ctx->r15 == ctx->r1) {
        // 0x8016A90C: lui         $at, 0x2
        ctx->r1 = S32(0X2 << 16);
        goto L_8016A964;
    }
    goto skip_13;
    // 0x8016A90C: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    skip_13:
    // 0x8016A910: lbu         $v0, 0x3F55($s1)
    ctx->r2 = MEM_BU(0X3F55, ctx->r17);
    // 0x8016A914: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x8016A918: lbu         $t8, 0x87($sp)
    ctx->r24 = MEM_BU(0X87, ctx->r29);
    // 0x8016A91C: bne         $v0, $at, L_8016A94C
    if (ctx->r2 != ctx->r1) {
        // 0x8016A920: addiu       $t2, $zero, 0xFF
        ctx->r10 = ADD32(0, 0XFF);
        goto L_8016A94C;
    }
    // 0x8016A920: addiu       $t2, $zero, 0xFF
    ctx->r10 = ADD32(0, 0XFF);
    // 0x8016A924: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(0X0, ctx->r17);
    // 0x8016A928: addu        $a0, $v0, $t8
    ctx->r4 = ADD32(ctx->r2, ctx->r24);
    // 0x8016A92C: jal         0x801323A0
    // 0x8016A930: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    Entrance_GetTransitionFlags(rdram, ctx);
    goto after_39;
    // 0x8016A930: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_39:
    // 0x8016A934: sra         $t9, $v0, 7
    ctx->r25 = S32(ctx->r2) >> 7;
    // 0x8016A938: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A93C: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A940: andi        $t1, $t9, 0x7F
    ctx->r9 = ctx->r25 & 0X7F;
    // 0x8016A944: b           L_8016A970
    // 0x8016A948: sb          $t1, -0x7781($at)
    MEM_B(-0X7781, ctx->r1) = ctx->r9;
    goto L_8016A970;
    // 0x8016A948: sb          $t1, -0x7781($at)
    MEM_B(-0X7781, ctx->r1) = ctx->r9;
L_8016A94C:
    // 0x8016A94C: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x8016A950: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A954: sb          $v0, -0x7781($at)
    MEM_B(-0X7781, ctx->r1) = ctx->r2;
    // 0x8016A958: b           L_8016A970
    // 0x8016A95C: sb          $t2, 0x3F55($s1)
    MEM_B(0X3F55, ctx->r17) = ctx->r10;
    goto L_8016A970;
    // 0x8016A95C: sb          $t2, 0x3F55($s1)
    MEM_B(0X3F55, ctx->r17) = ctx->r10;
    // 0x8016A960: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
L_8016A964:
    // 0x8016A964: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016A968: addiu       $t3, $zero, 0x2
    ctx->r11 = ADD32(0, 0X2);
    // 0x8016A96C: sb          $t3, -0x7781($at)
    MEM_B(-0X7781, ctx->r1) = ctx->r11;
L_8016A970:
    // 0x8016A970: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016A974: ori         $at, $at, 0x8E48
    ctx->r1 = ctx->r1 | 0X8E48;
    // 0x8016A978: addu        $a0, $s0, $at
    ctx->r4 = ADD32(ctx->r16, ctx->r1);
    // 0x8016A97C: jal         0x8016482C
    // 0x8016A980: sw          $a0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r4;
    TransitionFade_Init(rdram, ctx);
    goto after_40;
    // 0x8016A980: sw          $a0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r4;
    after_40:
    // 0x8016A984: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    // 0x8016A988: jal         0x80164A98
    // 0x8016A98C: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    TransitionFade_SetType(rdram, ctx);
    goto after_41;
    // 0x8016A98C: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    after_41:
    // 0x8016A990: lui         $a1, 0xA0A0
    ctx->r5 = S32(0XA0A0 << 16);
    // 0x8016A994: ori         $a1, $a1, 0xA0FF
    ctx->r5 = ctx->r5 | 0XA0FF;
    // 0x8016A998: jal         0x80164A8C
    // 0x8016A99C: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    TransitionFade_SetColor(rdram, ctx);
    goto after_42;
    // 0x8016A99C: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    after_42:
    // 0x8016A9A0: jal         0x801647D0
    // 0x8016A9A4: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    TransitionFade_Start(rdram, ctx);
    goto after_43;
    // 0x8016A9A4: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    after_43:
    // 0x8016A9A8: lui         $a0, 0x801F
    ctx->r4 = S32(0X801F << 16);
    // 0x8016A9AC: jal         0x801418B0
    // 0x8016A9B0: addiu       $a0, $a0, 0x6D18
    ctx->r4 = ADD32(ctx->r4, 0X6D18);
    VisMono_Init(rdram, ctx);
    goto after_44;
    // 0x8016A9B0: addiu       $a0, $a0, 0x6D18
    ctx->r4 = ADD32(ctx->r4, 0X6D18);
    after_44:
    // 0x8016A9B4: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x8016A9B8: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8016A9BC: lui         $t4, 0x801F
    ctx->r12 = S32(0X801F << 16);
    // 0x8016A9C0: addiu       $v0, $v0, 0x6D4C
    ctx->r2 = ADD32(ctx->r2, 0X6D4C);
    // 0x8016A9C4: sb          $zero, 0x6D33($at)
    MEM_B(0X6D33, ctx->r1) = 0;
    // 0x8016A9C8: addiu       $a0, $t4, 0x6D38
    ctx->r4 = ADD32(ctx->r12, 0X6D38);
    // 0x8016A9CC: jal         0x80140E80
    // 0x8016A9D0: sw          $a0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r4;
    VisFbuf_Init(rdram, ctx);
    goto after_45;
    // 0x8016A9D0: sw          $a0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r4;
    after_45:
    // 0x8016A9D4: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x8016A9D8: addiu       $v0, $v0, 0x6D4C
    ctx->r2 = ADD32(ctx->r2, 0X6D4C);
    // 0x8016A9DC: lw          $t5, 0x0($v0)
    ctx->r13 = MEM_W(0X0, ctx->r2);
    // 0x8016A9E0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016A9E4: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x8016A9E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016A9EC: swc1        $f4, 0x8($t5)
    MEM_W(0X8, ctx->r13) = ctx->f4.u32l;
    // 0x8016A9F0: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(0X0, ctx->r2);
    // 0x8016A9F4: sb          $t6, 0x0($t7)
    MEM_B(0X0, ctx->r15) = ctx->r14;
    // 0x8016A9F8: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(0X0, ctx->r2);
    // 0x8016A9FC: sb          $zero, 0xC($t8)
    MEM_B(0XC, ctx->r24) = 0;
    // 0x8016AA00: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(0X0, ctx->r2);
    // 0x8016AA04: sb          $zero, 0xD($t9)
    MEM_B(0XD, ctx->r25) = 0;
    // 0x8016AA08: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(0X0, ctx->r2);
    // 0x8016AA0C: sb          $zero, 0xE($t1)
    MEM_B(0XE, ctx->r9) = 0;
    // 0x8016AA10: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(0X0, ctx->r2);
    // 0x8016AA14: sb          $zero, 0xF($t2)
    MEM_B(0XF, ctx->r10) = 0;
    // 0x8016AA18: lw          $t3, 0x0($v0)
    ctx->r11 = MEM_W(0X0, ctx->r2);
    // 0x8016AA1C: sb          $zero, 0x10($t3)
    MEM_B(0X10, ctx->r11) = 0;
    // 0x8016AA20: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(0X0, ctx->r2);
    // 0x8016AA24: sb          $zero, 0x11($t4)
    MEM_B(0X11, ctx->r12) = 0;
    // 0x8016AA28: lw          $t5, 0x0($v0)
    ctx->r13 = MEM_W(0X0, ctx->r2);
    // 0x8016AA2C: sb          $zero, 0x12($t5)
    MEM_B(0X12, ctx->r13) = 0;
    // 0x8016AA30: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(0X0, ctx->r2);
    // 0x8016AA34: jal         0x800F12D0
    // 0x8016AA38: sb          $zero, 0x13($t6)
    MEM_B(0X13, ctx->r14) = 0;
    CutsceneFlags_UnsetAll(rdram, ctx);
    goto after_46;
    // 0x8016AA38: sb          $zero, 0x13($t6)
    MEM_B(0X13, ctx->r14) = 0;
    after_46:
    // 0x8016AA3C: addiu       $a0, $s0, 0x74
    ctx->r4 = ADD32(ctx->r16, 0X74);
    // 0x8016AA40: jal         0x80172B04
    // 0x8016AA44: sw          $a0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r4;
    THA_GetRemaining(rdram, ctx);
    goto after_47;
    // 0x8016AA44: sw          $a0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r4;
    after_47:
    // 0x8016AA48: jal         0x80172B04
    // 0x8016AA4C: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    THA_GetRemaining(rdram, ctx);
    goto after_48;
    // 0x8016AA4C: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    after_48:
    // 0x8016AA50: sw          $v0, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->r2;
    // 0x8016AA54: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    // 0x8016AA58: jal         0x80172AC8
    // 0x8016AA5C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    THA_AllocTailAlign16(rdram, ctx);
    goto after_49;
    // 0x8016AA5C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_49:
    // 0x8016AA60: lw          $t7, 0x94($sp)
    ctx->r15 = MEM_W(0X94, ctx->r29);
    // 0x8016AA64: addiu       $a2, $v0, 0x8
    ctx->r6 = ADD32(ctx->r2, 0X8);
    // 0x8016AA68: addiu       $at, $zero, -0x10
    ctx->r1 = ADD32(0, -0X10);
    // 0x8016AA6C: and         $a2, $a2, $at
    ctx->r6 = ctx->r6 & ctx->r1;
    // 0x8016AA70: subu        $t8, $t7, $a2
    ctx->r24 = SUB32(ctx->r15, ctx->r6);
    // 0x8016AA74: addu        $a1, $t8, $v0
    ctx->r5 = ADD32(ctx->r24, ctx->r2);
    // 0x8016AA78: jal         0x80102DBC
    // 0x8016AA7C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    ZeldaArena_Init(rdram, ctx);
    goto after_50;
    // 0x8016AA7C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_50:
    // 0x8016AA80: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016AA84: ori         $at, $at, 0x8000
    ctx->r1 = ctx->r1 | 0X8000;
    // 0x8016AA88: addu        $v0, $s0, $at
    ctx->r2 = ADD32(ctx->r16, ctx->r1);
    // 0x8016AA8C: lw          $a2, 0x850($v0)
    ctx->r6 = MEM_W(0X850, ctx->r2);
    // 0x8016AA90: addiu       $a1, $s0, 0x1CA0
    ctx->r5 = ADD32(ctx->r16, 0X1CA0);
    // 0x8016AA94: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x8016AA98: sw          $v0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r2;
    // 0x8016AA9C: jal         0x800B9170
    // 0x8016AAA0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Actor_InitContext(rdram, ctx);
    goto after_51;
    // 0x8016AAA0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_51:
    // 0x8016AAA4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016AAA8: ori         $at, $at, 0x86E0
    ctx->r1 = ctx->r1 | 0X86E0;
    // 0x8016AAAC: addu        $a1, $s0, $at
    ctx->r5 = ADD32(ctx->r16, ctx->r1);
    // 0x8016AAB0: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x8016AAB4: jal         0x8012EAA8
    // 0x8016AAB8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Room_HandleLoadCallbacks(rdram, ctx);
    goto after_52;
    // 0x8016AAB8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_52:
    // 0x8016AABC: bnel        $v0, $zero, L_8016AADC
    if (ctx->r2 != 0) {
        // 0x8016AAC0: lw          $t9, 0x18($s1)
        ctx->r25 = MEM_W(0X18, ctx->r17);
        goto L_8016AADC;
    }
    goto skip_14;
    // 0x8016AAC0: lw          $t9, 0x18($s1)
    ctx->r25 = MEM_W(0X18, ctx->r17);
    skip_14:
    // 0x8016AAC4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
L_8016AAC8:
    // 0x8016AAC8: jal         0x8012EAA8
    // 0x8016AACC: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(0X3C, ctx->r29);
    Room_HandleLoadCallbacks(rdram, ctx);
    goto after_53;
    // 0x8016AACC: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(0X3C, ctx->r29);
    after_53:
    // 0x8016AAD0: beql        $v0, $zero, L_8016AAC8
    if (ctx->r2 == 0) {
        // 0x8016AAD4: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
        goto L_8016AAC8;
    }
    goto skip_15;
    // 0x8016AAD4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    skip_15:
    // 0x8016AAD8: lw          $t9, 0x18($s1)
    ctx->r25 = MEM_W(0X18, ctx->r17);
L_8016AADC:
    // 0x8016AADC: addiu       $v1, $zero, 0x5
    ctx->r3 = ADD32(0, 0X5);
    // 0x8016AAE0: lw          $t2, 0x48($sp)
    ctx->r10 = MEM_W(0X48, ctx->r29);
    // 0x8016AAE4: div         $zero, $t9, $v1
    lo = S32(S64(S32(ctx->r25)) / S64(S32(ctx->r3))); hi = S32(S64(S32(ctx->r25)) % S64(S32(ctx->r3)));
    // 0x8016AAE8: bne         $v1, $zero, L_8016AAF4
    if (ctx->r3 != 0) {
        // 0x8016AAEC: nop
    
        goto L_8016AAF4;
    }
    // 0x8016AAEC: nop

    // 0x8016AAF0: break       7
    do_break(2148969200);
L_8016AAF4:
    // 0x8016AAF4: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016AAF8: bne         $v1, $at, L_8016AB0C
    if (ctx->r3 != ctx->r1) {
        // 0x8016AAFC: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
        goto L_8016AB0C;
    }
    // 0x8016AAFC: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x8016AB00: bne         $t9, $at, L_8016AB0C
    if (ctx->r25 != ctx->r1) {
        // 0x8016AB04: nop
    
        goto L_8016AB0C;
    }
    // 0x8016AB04: nop

    // 0x8016AB08: break       6
    do_break(2148969224);
L_8016AB0C:
    // 0x8016AB0C: mfhi        $t1
    ctx->r9 = hi;
    // 0x8016AB10: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8016AB14: beql        $t1, $zero, L_8016AB60
    if (ctx->r9 == 0) {
        // 0x8016AB18: lw          $a1, 0x1CCC($s0)
        ctx->r5 = MEM_W(0X1CCC, ctx->r16);
        goto L_8016AB60;
    }
    goto skip_16;
    // 0x8016AB18: lw          $a1, 0x1CCC($s0)
    ctx->r5 = MEM_W(0X1CCC, ctx->r16);
    skip_16:
    // 0x8016AB1C: lbu         $v0, 0x6E3($t2)
    ctx->r2 = MEM_BU(0X6E3, ctx->r10);
    // 0x8016AB20: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(0X4C, ctx->r29);
    // 0x8016AB24: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8016AB28: beq         $v0, $at, L_8016AB38
    if (ctx->r2 == ctx->r1) {
        // 0x8016AB2C: addiu       $a2, $zero, 0x15A
        ctx->r6 = ADD32(0, 0X15A);
        goto L_8016AB38;
    }
    // 0x8016AB2C: addiu       $a2, $zero, 0x15A
    ctx->r6 = ADD32(0, 0X15A);
    // 0x8016AB30: bnel        $v1, $v0, L_8016AB60
    if (ctx->r3 != ctx->r2) {
        // 0x8016AB34: lw          $a1, 0x1CCC($s0)
        ctx->r5 = MEM_W(0X1CCC, ctx->r16);
        goto L_8016AB60;
    }
    goto skip_17;
    // 0x8016AB34: lw          $a1, 0x1CCC($s0)
    ctx->r5 = MEM_W(0X1CCC, ctx->r16);
    skip_17:
L_8016AB38:
    // 0x8016AB38: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8016AB3C: sw          $zero, 0x18($sp)
    MEM_W(0X18, ctx->r29) = 0;
    // 0x8016AB40: sw          $zero, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = 0;
    // 0x8016AB44: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x8016AB48: sw          $zero, 0x20($sp)
    MEM_W(0X20, ctx->r29) = 0;
    // 0x8016AB4C: sw          $zero, 0x24($sp)
    MEM_W(0X24, ctx->r29) = 0;
    // 0x8016AB50: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
    // 0x8016AB54: jal         0x800BAC60
    // 0x8016AB58: swc1        $f0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f0.u32l;
    Actor_Spawn(rdram, ctx);
    goto after_54;
    // 0x8016AB58: swc1        $f0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f0.u32l;
    after_54:
    // 0x8016AB5C: lw          $a1, 0x1CCC($s0)
    ctx->r5 = MEM_W(0X1CCC, ctx->r16);
L_8016AB60:
    // 0x8016AB60: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(0X40, ctx->r29);
    // 0x8016AB64: jal         0x800DE0EC
    // 0x8016AB68: sw          $a1, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r5;
    Camera_InitFocalActorSettings(rdram, ctx);
    goto after_55;
    // 0x8016AB68: sw          $a1, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r5;
    after_55:
    // 0x8016AB6C: lw          $a1, 0x90($sp)
    ctx->r5 = MEM_W(0X90, ctx->r29);
    // 0x8016AB70: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x8016AB74: sw          $zero, 0xD50($at)
    MEM_W(0XD50, ctx->r1) = 0;
    // 0x8016AB78: lh          $a2, 0x1C($a1)
    ctx->r6 = MEM_H(0X1C, ctx->r5);
    // 0x8016AB7C: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x8016AB80: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(0X40, ctx->r29);
    // 0x8016AB84: andi        $a2, $a2, 0xFF
    ctx->r6 = ctx->r6 & 0XFF;
    // 0x8016AB88: beq         $a2, $at, L_8016AB98
    if (ctx->r6 == ctx->r1) {
        // 0x8016AB8C: nop
    
        goto L_8016AB98;
    }
    // 0x8016AB8C: nop

    // 0x8016AB90: jal         0x800DFB14
    // 0x8016AB94: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    Camera_ChangeActorCsCamIndex(rdram, ctx);
    goto after_56;
    // 0x8016AB94: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_56:
L_8016AB98:
    // 0x8016AB98: jal         0x800F15D8
    // 0x8016AB9C: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(0X40, ctx->r29);
    CutsceneManager_StoreCamera(rdram, ctx);
    goto after_57;
    // 0x8016AB9C: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(0X40, ctx->r29);
    after_57:
    // 0x8016ABA0: jal         0x801129E4
    // 0x8016ABA4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Interface_SetSceneRestrictions(rdram, ctx);
    goto after_58;
    // 0x8016ABA4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_58:
    // 0x8016ABA8: jal         0x800FB758
    // 0x8016ABAC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Environment_PlaySceneSequence(rdram, ctx);
    goto after_59;
    // 0x8016ABAC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_59:
    // 0x8016ABB0: lbu         $t3, 0x814($s0)
    ctx->r11 = MEM_BU(0X814, ctx->r16);
    // 0x8016ABB4: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8016ABB8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016ABBC: sb          $t3, 0x3586($at)
    MEM_B(0X3586, ctx->r1) = ctx->r11;
    // 0x8016ABC0: lbu         $t4, 0x815($s0)
    ctx->r12 = MEM_BU(0X815, ctx->r16);
    // 0x8016ABC4: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8016ABC8: lw          $a1, 0x44($sp)
    ctx->r5 = MEM_W(0X44, ctx->r29);
    // 0x8016ABCC: jal         0x80135EE8
    // 0x8016ABD0: sb          $t4, 0x3587($at)
    MEM_B(0X3587, ctx->r1) = ctx->r12;
    AnimationContext_Update(rdram, ctx);
    goto after_60;
    // 0x8016ABD0: sb          $t4, 0x3587($at)
    MEM_B(0X3587, ctx->r1) = ctx->r12;
    after_60:
    // 0x8016ABD4: jal         0x800EDBE0
    // 0x8016ABD8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    Cutscene_HandleEntranceTriggers(rdram, ctx);
    goto after_61;
    // 0x8016ABD8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_61:
    // 0x8016ABDC: sw          $zero, 0x3CB0($s1)
    MEM_W(0X3CB0, ctx->r17) = 0;
    // 0x8016ABE0: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8016ABE4: lui         $a0, 0x801F
    ctx->r4 = S32(0X801F << 16);
    // 0x8016ABE8: sb          $zero, 0x6DFC($at)
    MEM_B(0X6DFC, ctx->r1) = 0;
    // 0x8016ABEC: jal         0x8016FC78
    // 0x8016ABF0: addiu       $a0, $a0, 0x6D50
    ctx->r4 = ADD32(ctx->r4, 0X6D50);
    BombersNotebook_Init(rdram, ctx);
    goto after_62;
    // 0x8016ABF0: addiu       $a0, $a0, 0x6D50
    ctx->r4 = ADD32(ctx->r4, 0X6D50);
    after_62:
    // 0x8016ABF4: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(0X34, ctx->r29);
L_8016ABF8:
    // 0x8016ABF8: lw          $s0, 0x2C($sp)
    ctx->r16 = MEM_W(0X2C, ctx->r29);
    // 0x8016ABFC: lw          $s1, 0x30($sp)
    ctx->r17 = MEM_W(0X30, ctx->r29);
    // 0x8016AC00: jr          $ra
    // 0x8016AC04: addiu       $sp, $sp, 0xA8
    ctx->r29 = ADD32(ctx->r29, 0XA8);
    return;
    // 0x8016AC04: addiu       $sp, $sp, 0xA8
    ctx->r29 = ADD32(ctx->r29, 0XA8);
;}
