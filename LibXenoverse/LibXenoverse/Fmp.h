#ifndef FMPFILE_H
#define FMPFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{


#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define FMP_SIGNATURE "#FMP"

// All offsets of file are absolute

struct FMPHeader
{
	char signature[4]; // 0
	uint32_t version; // 4
	uint32_t unk_08; // must be an even number. [A] always 0x12
	uint32_t unk_0C; // [A] always 0
	uint32_t unk0A_offset; // Offset ([A] always 0x70 because of header Size)
	uint32_t unk0B_offset; // Offset ([A] always 0xFC)
	uint32_t num_unk1; // 0x18
	uint32_t unk1_offset; // 0x1C
	uint32_t num_unk2; // 0x20
	uint32_t unk2_offset; // 0x24
	uint32_t num_unk3; // 0x28
	uint32_t unk3_offset; // 0x2C
	uint32_t num_unk4; // 0x30
	uint32_t unk4_offset; // 0x34;
	uint32_t num_unk5; // 0x38
	uint32_t unk5_offset; // 0x3C
	uint32_t num_dep1; // 0x40						//=>  dans les Qst et EVe, il n'y en a pas, et dans le BFten.map, il y a des noms ajouté : du style les noms de fichiers, de model, "Start Frame" "Speed Rate", "PLAY SOUND 3D" , etc ...
	uint32_t dep1_table_offset; // 0x44
	uint32_t num_dep2; // 0x48
	uint32_t dep2_table_offset; // 0x4C
	uint32_t num_dep3; // 0x50
	uint32_t dep3_table_offset; // 0x54
	uint32_t num_dep4; // 0x58
	uint32_t dep4_table_offset; // 0x5C
	uint32_t unk_60[4]; // Found to be zero  but game code references first two. [A] always 0
} PACKED;
static_assert(sizeof(FMPHeader) == 0x70, "Incorrect structure size.");



// 70 -> FC => 8C = 140 = 35 * 4. d'ou vient les 35 valeurs ? Note: on a 0x12 (36) comme valeur inconue, ca a peut etre un rapport ?
struct FMPUnk0AEntry
{
	uint32_t unk_0;						// 0	=> 00 40 1C 46 (10000 en float ) (si couleur vert sombre, tres transparent 0.274 )
	uint32_t unk_1;						// 4	=> [A] always as unk_0
	uint32_t unk_2;						// 8	=> [A] always as unk_0

	uint32_t unk_3;						// C	=> [A] always  0
	uint32_t unk_4;						// 10	=> [A] always  0
	uint32_t unk_5;						// 14	=> [A] always  0

	uint32_t unk_6;						// 18	=> CD CC 1C C1  (-9, 8000001907349 en float), ca pourrait etre une couleur (vert caca d'oie, un peu transparente).
	uint32_t unk_7;						// 1C	=> [A] always 4			(5,6051938572993e-045 si float)

	uint32_t unk_8;						// 20	=> 3A CD 13 3F, (si couleur, vert tres transparent 0.24 ) (0,57735025882721 si float)
	uint32_t unk_9;						// 24	=> 3A CD 13 BF, (si couleur, vert un peu transparent ) (-0,57735025882721  si float)
	uint32_t unk_10;					// 28	=> 3A CD 13 3F, (si couleur, vert tres transparent 0.24 ) (0,57735025882721 si float)

	uint32_t unk_11;					// 2C	=> [A] always 9A 99 99 3E, (si couleur, gris tres transparent 0.24 ) (0,30000001192093 si float)
	uint32_t unk_12;					// 30	=> [A] always 9A 99 99 3E, (si couleur, gris tres transparent 0.24 ) (0,30000001192093 si float)
	uint32_t unk_13;					// 34	=> [A] always 9A 99 99 3E, (si couleur, gris tres transparent 0.24 ) (0,30000001192093 si float)

	uint32_t unk_14;					// 38	=> [A] always 33 33 33 3F, (si couleur, gris sombre tres transparent 0.24 ) (0,69999998807907 si float)
	uint32_t unk_15;					// 3C	=> [A] always 33 33 33 3F, (si couleur, gris sombre tres transparent 0.24 ) (0,69999998807907 si float)
	uint32_t unk_16;					// 40	=> [A] always 33 33 33 3F, (si couleur, gris sombre tres transparent 0.24 ) (0,69999998807907 si float)

	uint32_t unk_17;					// 44	=> [A] always 00 00 C8 42, (si couleur, bleu tres transparent) (100 si float)
	uint32_t unk_18;					// 48	=> [A] always 00 00 FA 43, (si couleur, bleu tres transparent) (500 si float)

	uint32_t unk_19;					// 4C	=> [A] always 0
	uint32_t unk_20;					// 50	=> [A] always 0
	uint32_t unk_21;					// 54	=> [A] always 0

	uint32_t unk_22;					// 58	=> [A] always 00 00 80 3F, (si couleur, bleu tres transparent) (1 si float)
	uint32_t unk_23;					// 5C	=> [A] always 00 00 80 3F, (si couleur, bleu tres transparent) (1 si float)
	uint32_t unk_24;					// 60	=> [A] always 00 00 80 3F, (si couleur, bleu tres transparent) (1 si float)

	uint32_t unk_25;					// 64	=> [A] always 00 00 C8 42, (si couleur, bleu tres transparent) (100 si float)
	uint32_t unk_26;					// 68	=> [A] always 00 00 96 43, (si couleur, bleu tres transparent) (300 si float)
	uint32_t unk_27;					// 6C	=> [A] always 00 00 96 42, (si couleur, bleu tres transparent) (75 si float)
	uint32_t unk_28;					// 70	=> [A] always 00 00 48 42, (si couleur, bleu tres transparent) (50 si float)

	uint32_t unk_29;					// 74	=> [A] always 00 00 00 3F, (si couleur, noir tres transparent) (0,5 si float)
	uint32_t unk_30;					// 78	=> [A] always 00 00 00 3F, (si couleur, noir tres transparent) (0,5 si float)
	uint32_t unk_31;					// 7C	=> [A] always 00 00 00 3F, (si couleur, noir tres transparent) (0,5 si float)

	uint32_t unk_32;					// 80	=> CD CC CC 3D, (si couleur, gris tres transparent) (0,10000000149012 si float)
	uint32_t unk_33;					// 84	=> 00 00 7A 44, (si couleur, bleu tres transparent) (1000 si float)
	uint32_t unk_34;					// 88	=> D8 0F 49 3F, (si couleur, rose/rouge tres transparent) (0,78539800643921 si float)

} PACKED;
static_assert(sizeof(FMPUnk0AEntry) == 0x8C, "Incorrect structure size.");






//todo terminer
// FC -> 2f8 => 1FC = 508

struct FMPUnk0BEntry
{
	uint32_t unk_0;						// 0	=> 18 61 B9 7C (7.7003537122912e+036 en float )
	uint32_t unk_1;						// 4	=> 0
	uint32_t unk_2;						// 8	=> 46 58 2E 17 (5,6333849001565e-025 en float )
	uint32_t unk_3;						// C	=> 0
	uint32_t unk_4;						// 10
	uint32_t unk_5;						// 14
	uint32_t unk_6;						// 18
	uint32_t unk_7;						// 1C
	uint32_t unk_8;						// 20
	uint32_t unk_9;						// 24
	uint32_t unk_10;					// 28
	uint32_t unk_11;					// 2C
	uint32_t unk_12;					// 30
	uint32_t unk_13;					// 34
	uint32_t unk_14;					// 38
	uint32_t unk_15;					// 3C
	uint32_t unk_16;					// 40
	uint32_t unk_17;					// 44
	uint32_t unk_18;					// 48
	uint32_t unk_19;					// 4C
	uint32_t unk_20;					// 50
	uint32_t unk_21;					// 54
	uint32_t unk_22;					// 58
	uint32_t unk_23;					// 5C
	uint32_t unk_24;					// 60
	uint32_t unk_25;					// 64
	uint32_t unk_26;					// 68
	uint32_t unk_27;					// 6C
	uint32_t unk_28;					// 70
	uint32_t unk_29;					// 74
	uint32_t unk_30;					// 78
	uint32_t unk_31;					// 7C
	uint32_t unk_32;					// 80
	uint32_t unk_33;					// 84
	uint32_t unk_34;					// 88
	uint32_t unk_35;					// 8C
	uint32_t unk_36;					// 90
	uint32_t unk_37;					// 94
	uint32_t unk_38;					// 98
	uint32_t unk_39;					// 9C
	uint32_t unk_40;					// A0
	uint32_t unk_41;					// A4
	uint32_t unk_42;					// A8
	uint32_t unk_43;					// AC
	uint32_t unk_44;					// B0
	uint32_t unk_45;					// B4
	uint32_t unk_46;					// B8
	uint32_t unk_47;					// BC
	uint32_t unk_48;					// C0
	uint32_t unk_49;					// C4
	uint32_t unk_50;					// C8
	uint32_t unk_51;					// CC
	uint32_t unk_52;					// D0
	uint32_t unk_53;					// D4
	uint32_t unk_54;					// D8
} PACKED;
static_assert(sizeof(FMPUnk0BEntry) == 0xDC, "Incorrect structure size.");

//+padding for having 256 (after FMPUnk0AEntry)
//after there is 51 nameOffset, may be some general types



struct FMPUnk1Entry
{
	uint32_t name_offset;			// [A] TRIGGER_BROAD_PHASE, TRIGGER_NARROW_PHASE, TRIGGER_CONTACT_SOLVER, Default, or some "__X"
	uint32_t unk_04;				// [A] 1, 2, 3 (increment), but at the end we have 0 for Default and "__X"
	float unk_08[4];				// [A] for unk_08[0-2] values are in [0, 1.0]. for unk_08[3] = 999936.0, 0.0 or 0.104279 => i'm not sure is a float value
} PACKED;
static_assert(sizeof(FMPUnk1Entry) == 0x18, "Incorrect structure size.");


struct FMPUnk2Entry
{
	uint32_t name_offset;			// [A] Default or some '__X'. => could refer to Unk1Entry by name (without knowed Triggers) => wrong (unk1E __118 with unk2E __105)
	float unk_04[6];				// we feel 0 and 1, 2 and 3, 4 and 5, are together. for unk_04[0] = 0.0, 1.0, 2.0 or 3.0. for unk_04[1] is always 1.0. for unk_04[2] = 200.0 or 300.0. for unk_04[3] = 100.0, or 300.0. for unk_04[4] is in [0,2.0]. for unk_04[5] is in [0, 10.0]. 
} PACKED;
static_assert(sizeof(FMPUnk2Entry) == 0x1C, "Incorrect structure size.");



struct FMPUnk3Entry
{
	uint32_t name_offset;			// [A] as 'stone', 'Gate', 'Tree', etc... there is some special : 'InitPos', 'Quest', 'Quest_X', 'Master_XXX' (name of Charac), 'hakai' (trad as 'destruction'), 'Fragments_X', 'EXP_OFF', 'Break_OFF', 'ACT', 'InstanceObject', 'EFCT', 'BG', 'REF', 'All', 'EVE', 'NPC_SH', 'Main', 'ObjIns_Palm', CLPC_POS, RECEIPT, FRIEND, TELEPORT, CONFLICT, SUBAREA, Collision, AREA_XXXXX, chase_battle, EMOTION, DB (for DragonBall), Respawn_Point, EVE_Point, Camera, GroupX, NPC_TALK, MainScenario, OnlineEventFs, TEACHER, 'Env Sound' . the fact as there is 'break' and 'Break' said it's not case sensitive.  a lot of name are for the free world, like shops, character for teachers ans npc. 
	uint32_t num_indexes;
	uint32_t indexes_offset;		// Points to table of 16 bits integers. Indexes are relative to Unk4Entrys
} PACKED;
static_assert(sizeof(FMPUnk3Entry) == 0xC, "Incorrect structure size.");


struct FMPUnk4Entry
{
	uint32_t name_offset;		// [A] as BFtok01, SUN, sound_ENV, HYPNO_TARGET, HYPNO_OWN, HYPNO_BACK_OWN, SKY, EFCT_smoke, GATE_to_XXXX (other part of the level), EFCT_Wind00_00, FragmentsFXX_XXXX (for sol grass). insStoneAcol, ULC_END_OWNER, ULC_END_MEMBER_X, ULC_END_HIT_X, ULC_END_GUARD_X, VS_INIT_POS_A_X, VS_INIT_POS_B_X, AI_MOVE_XX, QUEST_FORMAT, TRESPASS_X, MSTSPASS_X, QUEST_ITEM_XX, _initPos, GATE_to_BFceb, GATE_EXP_IN, GATE_EXP_OUT_0, LOCALBATTLE, insGrassX, ULC_XX_END_OWNER, ULC_XX_END_MEMBER_X, LOCAL_VS_INIT_POS_1P, LOCAL_VS_INIT_POS_2P, RISAN_BLOW_FIRE_PORT_X, MST_GOK_00_POS_XX, bnSKY, ENVTEX, Initialize, SetShadowArea, hRISAN_Worm_Hole_0, HYPERSHOT_00, HYPERSHOT_return, WATER, REF00,  EFCT_TakiA, ACT06_hune, ACT06_Event, bnTreeset, BFiceh97_TreeXXX, Taki_Collision, COMa10, COMa10_XXX, 00_ivent, 00_ivent_XXX, sound_fall, QUEST_DB_COLLECT (the space and time ship), EFCT_ICE_ID0020_01, GPUParticle, NG_GNT_001, watersurface, Event_water (when we go through ?), QUEST_ITEM_XX, Initiarize, EFCT_glowflyXX, NoiseTEX, BFkaiGrassMassX, GATE_toXX_float, GATE_toXX_stand, SmallScene, LBY10, AI_MOVE_XX, (next is for free world) PROLOGUE_STANDBY_TKT, PLAYERPOS_01, PROLOGUE_PLAYERPOS_00, FLYINGLOCK_PLAYER_00, TUTORIAL_POINT_PC, TUTORIAL_POINT_NPC, POINT_COMEBACK_TOKI, Hologram, MOVE_TO_LBY04, RESPAWNPOS_XX, CAMERA_AREA_OUT_00, WARPPOS_XX, NPC_TELEPORT_name_0100_00 (the name displayed up to geometry), NPC_TEACHER_XXX, X_Env_Sound_XX_X, Env_Sound_XX_X
	uint32_t unk_04;			// [A] 0, 1, 0x10001 =>  sort of flag. for the right bit, may be 0 for instanced objects and particules. 1 for the rest (so could be a bool 'instanced')
	uint16_t unk_08;			// [A] always 0xffff if unk_04==0. else it 'increment' but not clear (loop, repetition). it could go very high, specialy on free world (-> 0x55e)
	int16_t unk5_index;			// Signed, compared against < 0
	
	uint32_t unk1_offset;		// 0xC   (0x50*n, n is get somehow from FMPUnk5Entry) => FMPUnk4Entry_Next (Notice many Unk4Entry could use the same offset)
	uint32_t unk2_offset;		// 0x10	=> to FMPUnk4Entry_Next3 (Notice many Unk4Entry could use the same offset)
	uint32_t unk_14;			//			[A] 0 1 or 2, so it's could be a type
	
	uint32_t unk3_offset;		// 0x18		=> Ref to FMPUnk4Entry_Next2 (Notice many Unk4Entry could use the same offset)
	uint32_t unk4_offset;		// 0x1C		=> Ref to FMPUnk4Entry_Next4 (Notice many Unk4Entry could use the same offset)
	uint32_t unk_20;			//			[A] only 0x3fffffff, 0x0066cccc, 0x00ff00cc, 0x35ffffff => could be flags
	float transformMatrix4x3[12];	//=> m00, m01, m02 / m10, , m11, , m12 / m20, , m21, , m22 / m30, , m31, , m32 (translation) /
} PACKED;
static_assert(sizeof(FMPUnk4Entry) == 0x54, "Incorrect structure size.");








struct FMPUnk4Entry_Next
{
	uint32_t count;				// 0			=> [A] 0, 1 or 2 (may be a type)
	uint32_t unk1_offset;		// 4			=> to FMPUnk4Entry_Next_b
	uint32_t unk2_offset;		// 8			=> to FMPUnk4Entry_Next_c

	uint32_t unk_0;				// C			=> 0
	uint32_t unk3_offset;		// 10			=> 0 => Ref to FMPUnk4Entry_Next_d

	uint32_t unk_2;				// 14			=> 00 00 FF FF  [A] only 0xffff0000, 1 or 0
	uint32_t unk_4;				// 18			=> 0
	uint32_t unk_5;				// 1C			=> 0
	uint32_t unk_6;				// 20			=> 00 00 80 3F => (1 en float)

	uint32_t unk_7;				// 24			=> 0
	uint32_t unk_8;				// 28			=> 0								=> unk_4
	uint32_t unk_9;				// 2C			=> 0								=> unk_5
	uint32_t unk_10;			// 30			=> 00 00 80 3F => (1 en float)		=> unk_6

	uint32_t unk_8b;			// 34			=> 0								=> unk_7
	uint32_t unk_9b;			// 38			=> 0								=> unk_8
	uint32_t unk_10b;			// 3C			=> 0								=> unk_9
	uint32_t unk_11b;			// 40			=> 00 00 80 3F => (1 en float)		=> unk_10

	uint32_t unk_12;			// 44			=> 0
	uint32_t unk_13;			// 48			=> 0
	uint32_t unk_14;			// 4C			=> 0
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next) == 0x50, "Incorrect structure size.");


struct FMPUnk4Entry_Next_b			//from FMPUnk4Entry_Next.unk1_offset
{
	uint32_t unk_0;				// 0			=>
	uint32_t unk_1;				// 4			=>
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next_b) == 0x8, "Incorrect structure size.");


struct FMPUnk4Entry_Next_c			//from FMPUnk4Entry_Next.unk2_offset
{
	uint32_t unk_0;				// 0			=>
	uint32_t unk_1;				// 4			=>
	uint32_t unk_2;				// 8			=>
	uint32_t unk_3;				// C			=>
	uint32_t unk_4;				// 10			=>
	uint32_t unk_5;				// 14			=>
	uint32_t unk_6;				// 18			=>
	uint32_t unk_7;				// 1C			=>
	uint32_t unk_8;				// 20			=>
	uint32_t unk_9;				// 24			=>
	uint32_t unk_10;			// 28			=>
	uint32_t unk_11;			// 2C			=>
	uint32_t unk_12;			// 30			=>
	uint32_t unk_13;			// 34			=>
	uint32_t unk_14;			// 38			=>
	uint32_t unk_15;			// 3C			=>
	uint32_t unk_16;			// 40			=>
	uint32_t unk_17;			// 44			=>
	uint32_t unk_18;			// 48			=>
	uint32_t unk_19;			// 4C			=>
	uint32_t unk_20;			// 50			=>
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next_c) == 0x54, "Incorrect structure size.");


struct FMPUnk4Entry_Next_d			//from FMPUnk4Entry_Next.unk3_offset
{
	uint32_t unk_0;				// 0			=>
	uint32_t unk_1;				// 4			=>
	uint32_t count;				// 8			=>
	uint32_t unk1_offset;		// C			=> Ref to FMPUnk4Entry_Next_e
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next_d) == 0x10, "Incorrect structure size.");


struct FMPUnk4Entry_Next_e			//from FMPUnk4Entry_Next_d.unk1_offset
{
	uint32_t name_offset;		// 0			=>
	uint32_t unk_0;				// 4			=>
	uint32_t count;				// 8			=>
	uint32_t unk1_offset;		// C			=> Ref to FMPUnk4Entry_Next_f
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next_e) == 0x10, "Incorrect structure size.");


struct FMPUnk4Entry_Next_f			//from FMPUnk4Entry_Next_e.unk1_offset
{
	uint32_t name_offset;		// 0			=>
	uint32_t unk_0;				// 4			=> if ==6, unk1_offset is a nameOffset. so it's a type.
	uint32_t unk1_offset;		// 8			=> Ref to a unique Value. it's a nameOffset if unk_0 == 6
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next_f) == 0xC, "Incorrect structure size.");







struct FMPUnk4Entry_Next3		//from FMPUnk4Entry.unk2_offset
{
	uint32_t unk_0;			// 50			=> 0	[A] only 0,1,2,3 or 4 (may be a type)
	uint32_t unk_1;			// 54			=> 2
	uint32_t count;			// 58			=> 0
	uint32_t unk1_offset;	// 5C			=> to FMPUnk4Entry_Next3_b
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next3) == 0x10, "Incorrect structure size.");


struct FMPUnk4Entry_Next3_b			//from FMPUnk4Entry_Next3.unk1_offset
{
	uint32_t name_offset;		// 0			=> 
	uint32_t unk_1;				// 4			=>
	uint32_t count;				// 8			=>
	uint32_t unk1_offset;		// C			=> to FMPUnk4Entry_Next3_c
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next3_b) == 0x10, "Incorrect structure size.");


struct FMPUnk4Entry_Next3_c			//from FMPUnk4Entry_Next3_b.unk1_offset
{
	uint32_t name_offset;		// 0			=> 
	uint32_t unk_1;				// 4			=>
	uint32_t name_offset2;		// 8			=>
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next3_c) == 0xC, "Incorrect structure size.");







struct FMPUnk4Entry_Next2
{
	uint32_t unk1_offset;		// 0			=> ref to FMPUnk4Entry_Next2_b

	uint32_t unk_0;				// 4
	uint32_t unk_1;				// 8
	uint32_t unk_2;				// C
	uint32_t unk_3;				// 10
	uint32_t unk_4;				// 14
	uint32_t unk_5;				// 18
	uint32_t unk_6;				// 1C
	uint32_t unk_7;				// 20
	uint32_t unk_8;				// 24
	uint32_t unk_9;				// 28
	uint32_t unk_10;			// 2C
	uint32_t unk_11;			// 30
	uint32_t unk_12;			// 34

} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next2) == 0x38, "Incorrect structure size.");


struct FMPUnk4Entry_Next2_b
{
	uint32_t name_offset;		// 0

	uint32_t unk_0;				// 4
	uint32_t unk_1;				// 8		=> type witch influence unk_2, unk_4 unk_11 is a ref offset (if ==2).
	uint32_t unk_2;				// C		=> if unk_1==2, Ref to a FMPUnk4Entry_Next2_c
	uint32_t unk_3;				// 10
	uint32_t unk_4;				// 14		=> if unk_1==2, Ref to a FMPUnk4Entry_Next2_c
	uint32_t unk_5;				// 18
	uint32_t unk_6;				// 1C
	uint32_t unk_7;				// 20
	uint32_t unk_8;				// 24
	uint32_t unk_9;				// 28
	uint32_t unk_10;			// 2C
	uint32_t unk_11;			// 30		=> if unk_1==2, Ref to a FMPUnk4Entry_Next2_c (may be it's another things because don't look like values from others).
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next2_b) == 0x34, "Incorrect structure size.");


struct FMPUnk4Entry_Next2_c
{
	uint32_t unk_0;				// 0
	uint32_t unk_1;				// 4
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next2_c) == 0x8, "Incorrect structure size.");






struct FMPUnk4Entry_Next4
{
	uint32_t count;				// 0			=> count for FMPUnk4Entry_Next4_c
	uint32_t count2;			// 4			=> count for FMPUnk4Entry_Next4_b

	uint32_t unk1_offset;		// 8			=> ref to FMPUnk4Entry_Next4_b
	uint32_t unk2_offset;		// C			=> ref to FMPUnk4Entry_Next4_d
	uint32_t unk3_offset;		// 10			=> ref to FMPUnk4Entry_Next4_c
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next4) == 0x14, "Incorrect structure size.");

struct FMPUnk4Entry_Next4_b
{
	uint32_t unk_0;				// 0
	uint32_t unk_1;				// 4
	uint32_t unk_2;				// 8
	uint32_t unk_3;				// C
	uint32_t unk_4;				// 10
	uint32_t unk_5;				// 14
	uint32_t unk_6;				// 18
	uint32_t unk_7;				// 1C
	uint32_t unk_8;				// 20
	uint32_t unk_9;				// 24
	uint32_t unk_10;			// 28
	uint32_t unk_11;			// 2C
	
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next4_b) == 0x30, "Incorrect structure size.");


struct FMPUnk4Entry_Next4_c
{
	uint32_t unk_0;				// 0
	uint32_t unk_1;				// 4
	uint32_t unk_2;				// 8
	uint32_t unk_3;				// C
	uint32_t unk_4;				// 10
	uint32_t unk_5;				// 14
	uint32_t unk_6;				// 18
	uint32_t unk_7;				// 1C
	uint32_t unk_8;				// 20

} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next4_c) == 0x24, "Incorrect structure size.");




struct FMPUnk4Entry_Next4_d
{
	uint16_t unk_0;				// 0		=> if 0020 -> followed by FMPUnk4Entry_Next4_e else is Indexes
	uint16_t count;				// 2		=> number of followed Index (with padding to match uint32_t)
	
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next4_d) == 0x4, "Incorrect structure size.");


struct FMPUnk4Entry_Next4_e					//could be recursive with FMPUnk4Entry_Next4_d
{
	uint32_t unk_1;				// 0
	uint32_t unk_2;				// 4
	uint32_t unk_3;				// 5

	uint32_t unk1_offset;		// C			=> ref to a FMPUnk4Entry_Next4_e or FMPUnk4Entry_Next4_f
	uint32_t unk2_offset;		// 10			=> ref to a FMPUnk4Entry_Next4_e or FMPUnk4Entry_Next4_f
	uint32_t unk3_offset;		// 14			=> ref to a FMPUnk4Entry_Next4_e or FMPUnk4Entry_Next4_f
	uint32_t unk4_offset;		// 18			=> ref to a FMPUnk4Entry_Next4_e or FMPUnk4Entry_Next4_f

	uint32_t unk_4;				// 1C
	uint32_t unk_5;				// 20
	uint32_t unk_6;				// 24
	uint32_t unk_7;				// 28
} PACKED;
static_assert(sizeof(FMPUnk4Entry_Next4_e) == 0x2C, "Incorrect structure size.");








struct FMPUnk5Entry
{
	uint32_t name_offset;	// 0			// [A] BFtok_colroot, SUN, sound_ENV, HYPNO_TARGET, HYPNO_OWN, SKY,  (sound like FMPUnk4Entry's names, witch do the link. notice there is more names on Unk4Entry) 
	uint32_t count;			// 4
	uint32_t unk1_offset;	// 8
} PACKED;
static_assert(sizeof(FMPUnk5Entry) == 0xC, "Incorrect structure size.");


//FMPUnk5Entry.unk1_offset => (le lock va jusqu'au name => 46 Box iddentifié bien repoarti. => ca voudrait dire (hyp une box par block) 46 block , ca veut dire que les 9200 block jusqu'au name (pas de padding apaprent) represente 46 block de 200 octect, soit 0xC8 )
//=> avec 46, on a bien un block par FMPUnk5Entry
struct FMPUnk5Entry_Box				//work for Box and Sphere for size
{
	uint32_t name_offset;	// 0			=> BOX			[A] BOX, SPHERE, and a lot of names (sometime the same as Unk5Entry's names, but there is more name in this list).
	
	uint16_t child;			// 4			=> FF FF => null
	uint16_t unk_a0;		// 6			=> FF FF => null	[A] only 0 of 0xffff, may be a Bool  (may be for size)
	uint16_t sibling;		// 8			=> FF FF => null
	uint16_t parent;		// A			=> FF FF => null	[A] only 0 of 0xffff
	uint16_t unk_a1;		// C			=> [A] only 0, 1, 2, 3 or 4
	uint16_t unk_a2;		// E			=> [A] always 0

	uint32_t unk1_offset;	// 10			=> ref to FMPUnk5Entry_Box_b
	uint32_t unk_0;			// 14			=> 0
	uint32_t count2;		// 18			=> 0 [A] value in [0, 0x53da], number of FMPUnk5Entry_Box_e
	uint32_t unk2_offset;	// 1C			=> 0 => Ref to FMPUnk5Entry_Box_e
	uint32_t count3;		// 20			=> 0 [A] count of series of uint16_t values (list of Id, may the one of each FMPUnk5Entry_Box_e (same number)). if not null (and other unknowed cases), the next value is the unk3_offset

} PACKED;
static_assert(sizeof(FMPUnk5Entry_Box) == 0x24, "Incorrect structure size.");


struct FMPUnk5Entry_Box_b
{
	uint32_t unk_0;			// 0			=> 0
	uint32_t unk1_offset;	// 4			=> ref to FMPUnk5Entry_Box_c
} PACKED;
static_assert(sizeof(FMPUnk5Entry_Box_b) == 0x8, "Incorrect structure size.");


struct FMPUnk5Entry_Box_c
{
	uint32_t unk_0;			// 0			=> 0
	uint32_t unk1_offset;	// 4			=> ref to a FMPUnk5Entry_Box_d
	uint32_t unk_1;			// 8			=> 0 [A] always 0
	uint32_t unk_2;			// C			=> 0 [A] always 0
	uint32_t size3;			// 10			=> 0 size in octets of part started by unk3_offset (seam to be a file inserted)
	uint32_t unk3_offset;	// 14			=> 0 start of a part (seam to be a file inserted)
	uint32_t unk2_offset;	// 18			=> ref to a FMPUnk5Entry_Box_d
	uint32_t unk_5;			// 1C			=> 0 [A] always 0
	uint32_t unk_6;			// 20			=> 0 [A] only 0 or 0x3c23d70a
	uint32_t unk_7;			// 24			=> 0A D7 23 3C ... ( 0.0099999997764826 si float ). [A] only 0, 1, 2 or 0x3c23d70a

} PACKED;
static_assert(sizeof(FMPUnk5Entry_Box_c) == 0x28, "Incorrect structure size.");



struct FMPUnk5Entry_Box_d
{
	uint32_t unk_15;		// 0			=> 1	[A] only 0, 1, 2, 3 or 0xb6800000
	uint32_t unk_16;		// 4			=> 0
	uint32_t unk_17;		// 8			=> 0
	uint32_t unk_18;		// C			=> 0
	uint32_t unk_19;		// 10			=> 0
	uint32_t unk_20;		// 14			=> -0.0 (generalement les 00 00 00 80 correspondent a des float -0.0)
	uint32_t unk_21;		// 18			=> 0
	uint32_t unk_22;		// 1C			=> 00 00 00 3F (0.5 si float)
	uint32_t unk_23;		// 20			=> 00 00 00 3F (0.5 si float)
	uint32_t unk_24;		// 24			=> 00 00 00 3F (0.5 si float)
	uint32_t unk_25;		// 28			=> 0 [A] always 0
	uint32_t unk_26;		// 2C			=> 0 [A] always 0
	uint32_t unk_27;		// 30			=> 0
	uint32_t unk_28;		// 34			=> 0 [A] only 0, 1, 2, 0x3054414d

} PACKED;
static_assert(sizeof(FMPUnk5Entry_Box_d) == 0x38, "Incorrect structure size.");




struct FMPUnk5Entry_Box_e
{
	uint32_t unk_0;		// 0			=> TODO
	uint32_t unk_1;		// 4			=> 
	uint32_t unk_2;		// 8			=> 
	uint32_t unk_3;		// C			=> 
	uint32_t unk_4;		// 10			=> 
	uint32_t unk_5;		// 14			=> 

} PACKED;
static_assert(sizeof(FMPUnk5Entry_Box_e) == 0x18, "Incorrect structure size.");




#ifdef _MSC_VER
#pragma pack(pop)
#endif


class FmpFile : public LibXenoverse::EMO_BaseFile
{

	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.

public:
	FmpFile();
	virtual ~FmpFile() { Reset(); }

	bool Load(const uint8_t *buf, size_t size);
	void Reset();

	void save_Xml(string filename, bool show_error = false);		// Xml version for debug.
	void write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size);
	

	void save_Coloration(string filename, bool show_error = false);		//create a file for wxHexEditor, for add tag and color on section of the file.
	void write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size);
	void write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList = (size_t)-1, bool checkAllreadyTaggued = true);
};

}

#endif // FMPFILE_H
