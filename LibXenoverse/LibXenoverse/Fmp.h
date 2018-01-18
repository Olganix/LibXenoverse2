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

struct FMP_Header
{
	char signature[4];			// 0
	uint32_t version;			// 4
	uint32_t unk_08;			// 8	must be an even number. [A] always 0x12
	uint32_t unk_0C;			// C	[A] always 0
	
	uint32_t offset_Section0a;	// 10	Offset ([A] always 0x70 because of header Size)
	uint32_t offset_Section0b;	// 14	Offset ([A] always 0xFC)
	uint32_t numberSection1;	// 18
	uint32_t offset_Section1;	// 1C
	uint32_t numberSection2;	// 20
	uint32_t offset_Section2;	// 24
	uint32_t numberSection3;	// 28
	uint32_t offset_Section3;	// 2C
	uint32_t numberSection4;	// 30
	uint32_t offset_Section4;	// 34;
	uint32_t numberSection5;	// 38
	uint32_t offset_Section5;	// 3C
	
	uint32_t numberFileInDependance_Nsk; // 40						//=>  dans les Qst et EVe, il n'y en a pas, et dans le BFten.map, il y a des noms ajouté : du style les noms de fichiers, de model, "Start Frame" "Speed Rate", "PLAY SOUND 3D" , etc ...
	uint32_t offset_Dependance_Nsk; // 44
	uint32_t numberFileInDependance_TextureEmb; // 48
	uint32_t offset_Dependance_TextureEmb; // 4C
	uint32_t numberFileInDependance_Emm; // 50
	uint32_t offset_Dependance_Emm; // 54
	uint32_t numberFileInDependance_Ema; // 58
	uint32_t offset_Dependance_Ema; // 5C
	
	uint32_t unk_60;			// 60 Found to be zero  but game code references first two. [A] always 0
	uint32_t unk_64;			// 64
	uint32_t unk_68;			// 68
	uint32_t unk_6C;			// 6C
} PACKED;
static_assert(sizeof(FMP_Header) == 0x70, "Incorrect structure size.");



// 70 -> FC => 8C = 140 = 35 * 4. d'ou vient les 35 valeurs ? Note: on a 0x12 (36) comme valeur inconue, ca a peut etre un rapport ?
struct FMP_Section0a
{
	float width_X;						// 0	=> 00 40 1C 46 (10000 en float ) (si couleur vert sombre, tres transparent 0.274 )
	float width_Y;						// 4	=> [A] always as unk_0
	float width_Z;						// 8	=> [A] always as unk_0

	float unk_3;						// C	=> [A] always  0
	float unk_4;						// 10	=> [A] always  0
	float unk_5;						// 14	=> [A] always  0

	float unk_6;						// 18	=> CD CC 1C C1  (-9, 8000001907349 en float), ca pourrait etre une couleur (vert caca d'oie, un peu transparente).
	uint32_t unk_7;						// 1C	=> [A] always 4			(5,6051938572993e-045 si float)

	float unk_8;						// 20	=> 3A CD 13 3F, (si couleur, vert tres transparent 0.24 ) (0,57735025882721 si float)
	float unk_9;						// 24	=> 3A CD 13 BF, (si couleur, vert un peu transparent ) (-0,57735025882721  si float)
	float unk_10;					// 28	=> 3A CD 13 3F, (si couleur, vert tres transparent 0.24 ) (0,57735025882721 si float)

	float unk_11;					// 2C	=> [A] always 9A 99 99 3E, (si couleur, gris tres transparent 0.24 ) (0,30000001192093 si float)
	float unk_12;					// 30	=> [A] always 9A 99 99 3E, (si couleur, gris tres transparent 0.24 ) (0,30000001192093 si float)
	float unk_13;					// 34	=> [A] always 9A 99 99 3E, (si couleur, gris tres transparent 0.24 ) (0,30000001192093 si float)

	float unk_14;					// 38	=> [A] always 33 33 33 3F, (si couleur, gris sombre tres transparent 0.24 ) (0,69999998807907 si float)
	float unk_15;					// 3C	=> [A] always 33 33 33 3F, (si couleur, gris sombre tres transparent 0.24 ) (0,69999998807907 si float)
	float unk_16;					// 40	=> [A] always 33 33 33 3F, (si couleur, gris sombre tres transparent 0.24 ) (0,69999998807907 si float)

	float unk_17;					// 44	=> [A] always 00 00 C8 42, (si couleur, bleu tres transparent) (100 si float)
	float unk_18;					// 48	=> [A] always 00 00 FA 43, (si couleur, bleu tres transparent) (500 si float)

	float unk_19;					// 4C	=> [A] always 0
	float unk_20;					// 50	=> [A] always 0
	float unk_21;					// 54	=> [A] always 0

	float unk_22;					// 58	=> [A] always 00 00 80 3F, (si couleur, bleu tres transparent) (1 si float)
	float unk_23;					// 5C	=> [A] always 00 00 80 3F, (si couleur, bleu tres transparent) (1 si float)
	float unk_24;					// 60	=> [A] always 00 00 80 3F, (si couleur, bleu tres transparent) (1 si float)

	float unk_25;					// 64	=> [A] always 00 00 C8 42, (si couleur, bleu tres transparent) (100 si float)
	float unk_26;					// 68	=> [A] always 00 00 96 43, (si couleur, bleu tres transparent) (300 si float)
	float unk_27;					// 6C	=> [A] always 00 00 96 42, (si couleur, bleu tres transparent) (75 si float)
	float unk_28;					// 70	=> [A] always 00 00 48 42, (si couleur, bleu tres transparent) (50 si float)

	float unk_29;					// 74	=> [A] always 00 00 00 3F, (si couleur, noir tres transparent) (0,5 si float)
	float unk_30;					// 78	=> [A] always 00 00 00 3F, (si couleur, noir tres transparent) (0,5 si float)
	float unk_31;					// 7C	=> [A] always 00 00 00 3F, (si couleur, noir tres transparent) (0,5 si float)

	float nearDistance;				// 80	=> CD CC CC 3D, (si couleur, gris tres transparent) (0,10000000149012 si float)
	float farDistance;				// 84	=> 00 00 7A 44, (si couleur, bleu tres transparent) (1000 si float)
	float unk_34;					// 88	=> D8 0F 49 3F, (si couleur, rose/rouge tres transparent) (0,78539800643921 si float)

} PACKED;
static_assert(sizeof(FMP_Section0a) == 0x8C, "Incorrect structure size.");






//todo terminer
// FC -> 2f8 => 1FC = 508

struct FMP_Section0b
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
	uint16_t unk_23;					// 5C
	uint16_t unk_23b;					// 5E
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
static_assert(sizeof(FMP_Section0b) == 0xDC, "Incorrect structure size.");

//+padding for having 256 (after Section0A)
//after there is 51 nameOffset, may be some general types



struct FMP_Section1
{
	uint32_t name_offset;			// 0	[A] TRIGGER_BROAD_PHASE, TRIGGER_NARROW_PHASE, TRIGGER_CONTACT_SOLVER, Default, or some "__X"
	uint32_t unk_04;				// 4	[A] 1, 2, 3 (increment), but at the end we have 0 for Default and "__X"
	float unk_08;					// 8	[A] values are in [0, 1.0]
	float unk_0C;					// C	[A] values are in [0, 1.0]
	float unk_10;					// 10	[A] values are in [0, 1.0]
	float unk_14;					// 14	[A] 999936.0, 0.0 or 0.104279 => i'm not sure is a float value
} PACKED;
static_assert(sizeof(FMP_Section1) == 0x18, "Incorrect structure size.");


struct FMP_Section2
{
	uint32_t name_offset;			// 0	[A] Default or some '__X'. => could refer to Unk1Entry by name (without knowed Triggers) => wrong (unk1E __118 with unk2E __105)
	float unk_04;					// 4	0.0, 1.0, 2.0 or 3.0. We feel 0 and 1, 2 and 3, 4 and 5, are together.
	float unk_08;					// 8	is always 1.0.
	float unk_0C;					// C	200.0 or 300.0
	float unk_10;					// 10	100.0, or 300.0. 
	float unk_14;					// 14	is in [0,2.0]
	float unk_18;					// 18	is in [0, 10.0]
} PACKED;
static_assert(sizeof(FMP_Section2) == 0x1C, "Incorrect structure size.");



struct FMP_Section3
{
	uint32_t name_offset;			// 0	[A] as 'stone', 'Gate', 'Tree', etc... there is some special : 'InitPos', 'Quest', 'Quest_X', 'Master_XXX' (name of Charac), 'hakai' (trad as 'destruction'), 'Fragments_X', 'EXP_OFF', 'Break_OFF', 'ACT', 'InstanceObject', 'EFCT', 'BG', 'REF', 'All', 'EVE', 'NPC_SH', 'Main', 'ObjIns_Palm', CLPC_POS, RECEIPT, FRIEND, TELEPORT, CONFLICT, SUBAREA, Collision, AREA_XXXXX, chase_battle, EMOTION, DB (for DragonBall), Respawn_Point, EVE_Point, Camera, GroupX, NPC_TALK, MainScenario, OnlineEventFs, TEACHER, 'Env Sound' . the fact as there is 'break' and 'Break' said it's not case sensitive.  a lot of name are for the free world, like shops, character for teachers ans npc. 
	uint32_t num_indexes;			// 4	
	uint32_t indexes_offset;		// 8	Points to table of 16 bits integers. Indexes are relative to Unk4Entrys
} PACKED;
static_assert(sizeof(FMP_Section3) == 0xC, "Incorrect structure size.");


// Here FMP_Section3_indexes











struct FMP_Section4
{
	uint32_t name_offset;		// 0	[A] as BFtok01, SUN, sound_ENV, HYPNO_TARGET, HYPNO_OWN, HYPNO_BACK_OWN, SKY, EFCT_smoke, GATE_to_XXXX (other part of the level), EFCT_Wind00_00, FragmentsFXX_XXXX (for sol grass). insStoneAcol, ULC_END_OWNER, ULC_END_MEMBER_X, ULC_END_HIT_X, ULC_END_GUARD_X, VS_INIT_POS_A_X, VS_INIT_POS_B_X, AI_MOVE_XX, QUEST_FORMAT, TRESPASS_X, MSTSPASS_X, QUEST_ITEM_XX, _initPos, GATE_to_BFceb, GATE_EXP_IN, GATE_EXP_OUT_0, LOCALBATTLE, insGrassX, ULC_XX_END_OWNER, ULC_XX_END_MEMBER_X, LOCAL_VS_INIT_POS_1P, LOCAL_VS_INIT_POS_2P, RISAN_BLOW_FIRE_PORT_X, MST_GOK_00_POS_XX, bnSKY, ENVTEX, Initialize, SetShadowArea, hRISAN_Worm_Hole_0, HYPERSHOT_00, HYPERSHOT_return, WATER, REF00,  EFCT_TakiA, ACT06_hune, ACT06_Event, bnTreeset, BFiceh97_TreeXXX, Taki_Collision, COMa10, COMa10_XXX, 00_ivent, 00_ivent_XXX, sound_fall, QUEST_DB_COLLECT (the space and time ship), EFCT_ICE_ID0020_01, GPUParticle, NG_GNT_001, watersurface, Event_water (when we go through ?), QUEST_ITEM_XX, Initiarize, EFCT_glowflyXX, NoiseTEX, BFkaiGrassMassX, GATE_toXX_float, GATE_toXX_stand, SmallScene, LBY10, AI_MOVE_XX, (next is for free world) PROLOGUE_STANDBY_TKT, PLAYERPOS_01, PROLOGUE_PLAYERPOS_00, FLYINGLOCK_PLAYER_00, TUTORIAL_POINT_PC, TUTORIAL_POINT_NPC, POINT_COMEBACK_TOKI, Hologram, MOVE_TO_LBY04, RESPAWNPOS_XX, CAMERA_AREA_OUT_00, WARPPOS_XX, NPC_TELEPORT_name_0100_00 (the name displayed up to geometry), NPC_TEACHER_XXX, X_Env_Sound_XX_X, Env_Sound_XX_X
	uint16_t unk_04;			// 4	[A] 0, 1, 0x10001 =>  sort of flag. for the right bit, may be 0 for instanced objects and particules. 1 for the rest (so could be a bool 'instanced')
	uint16_t unk_04b;			// 4	[A] 0, 1, 0x10001 =>  sort of flag. for the right bit, may be 0 for instanced objects and particules. 1 for the rest (so could be a bool 'instanced')
	uint16_t index_Section5;		// 8	[A] always 0xffff if unk_04==0.
	
	int8_t  unk_0A;					// a	Signed, compared against < 0
	int8_t  unk_0B;					// b	Signed, compared against < 0
	uint32_t offset_Section4_Next;	// C   (0x50*n, n is get somehow from Section5) => Section4_Next (Notice many Section4 could use the same offset)
	uint32_t offset_Section4_Next3;	// 10	=> to Section4_Next3 (Notice many Section4 could use the same offset)

	uint32_t number_Section4_Next2;	// 14	[A] 0 1 or 2, so it's could be a type
	uint32_t offset_Section4_Next2;	// 18	=> Ref to Section4_Next2 (Notice many Section4 could use the same offset)
	uint32_t offset_Section4_Next4;	// 1C	=> Ref to Section4_Next4 (Notice many Section4 could use the same offset)
	float unk_20;					// 20
	float transformMatrix4x3[12];	//=> m00, m01, m02 / m10, , m11, , m12 / m20, , m21, , m22 / m30, , m31, , m32 (translation) /
} PACKED;
static_assert(sizeof(FMP_Section4) == 0x54, "Incorrect structure size.");





struct FMP_Section4_Next
{
	uint32_t number_Section4_Next_b; // 0		=> [A] 0, 1 or 2 (may be a type)
	uint32_t offset_Section4_Next_b; // 4		=> to Section4_Next_b
	
	uint32_t offset_Section4_Next_c; // 8		=> to Section4_Next_c
	uint32_t offset_Section4_Next_c_2; // C		=> another to Section4_Next_c
	uint32_t offset_Section4_Next_d; // 10		=> 0 => Ref to Section4_Next_d

	uint16_t unk_2;				// 14			=> only 1  or 0
	uint16_t unk_2b;			// 16			=> only FFFF or 0
	float unk_4;				// 18			=> 0
	float unk_5;				// 1C			=> 0

	float transformMatrix4x3[12];	// 20 => m00, m01, m02 / m10, , m11, , m12 / m20, , m21, , m22 / m30, , m31, , m32 (translation) /
} PACKED;
static_assert(sizeof(FMP_Section4_Next) == 0x50, "Incorrect structure size.");


struct FMP_Section4_Next_b			//from Section4_Next.unk1_offset
{
	uint32_t unk_0;				// 0			=>
	uint32_t unk_1;				// 4			=>
} PACKED;
static_assert(sizeof(FMP_Section4_Next_b) == 0x8, "Incorrect structure size.");


struct FMP_Section4_Next_c			//from Section4_Next.unk2_offset
{
	uint16_t unk_0;				// 0			=> 
	uint16_t unk_0b;			// 2			=> 
	uint16_t unk_1;				// 4			=>
	uint16_t unk_1b;			// 6			=>
	uint32_t unk_2;				// 8			=>
	float unk_3;				// C			=>
	float unk_4;				// 10			=>
	float unk_5;				// 14			=>
	float unk_6;				// 18			=>
	float unk_7;				// 1C			=>
	float unk_8;				// 20			=>
	float unk_9;				// 24			=>
	float unk_10;				// 28			=>
	float unk_11;				// 2C			=>
	float unk_12;				// 30			=>
	float unk_13;				// 34			=> lot of high values ... strange
	float widthX;				// 38			=> may be widthX of the Area witch the gate is targetable/usable (in case of gate)
	float widthY;				// 3C			=> 
	float widthZ;				// 40			=> 
	float quaternionX;			// 44			=> 
	float quaternionY;			// 48			=> 
	float quaternionZ;			// 4C			=> 
	float quaternionW;			// 50			=> 
} PACKED;
static_assert(sizeof(FMP_Section4_Next_c) == 0x54, "Incorrect structure size.");


struct FMP_Section4_Next_d			//from Section4_Next.unk3_offset
{
	uint32_t unk_0;				// 0			=>
	uint8_t unk_1a;				// 4			=>
	uint8_t unk_1b;				// 5			=>
	uint8_t unk_1c;				// 6			=>
	uint8_t unk_1d;				// 7			=>
	uint32_t number_Section4_Next_e; // 8		=>
	uint32_t offset_Section4_Next_e; // C		=> Ref to Section4_Next_e
} PACKED;
static_assert(sizeof(FMP_Section4_Next_d) == 0x10, "Incorrect structure size.");


struct FMP_Section4_Next_e			//from Section4_Next_d.unk1_offset
{
	uint32_t name_offset;		// 0			=>
	uint8_t unk_1a;				// 4			=>
	uint8_t unk_1b;				// 5			=>
	uint8_t unk_1c;				// 6			=>
	uint8_t unk_1d;				// 7			=>
	uint32_t number_Section4_Next_f; // 8		=>
	uint32_t offset_Section4_Next_f; // C		=> Ref to Section4_Next_f
} PACKED;
static_assert(sizeof(FMP_Section4_Next_e) == 0x10, "Incorrect structure size.");


struct FMP_Section4_Next_f			//list of parameters
{
	uint32_t name_offset;		// 0			=>
	uint32_t type_Data;			// 4			=> if ==6, unk1_offset is a nameOffset. so it's a type. 0: bool , 1: uint, 2  float
	uint32_t offset_Data;		// 8			=> Ref to a unique Value. it's a nameOffset if unk_0 == 6
} PACKED;
static_assert(sizeof(FMP_Section4_Next_f) == 0xC, "Incorrect structure size.");







struct FMP_Section4_Next3		//from Section4.unk2_offset
{
	uint32_t unk_0;			// 0			=> 0	[A] only 0,1,2,3 or 4 (may be a type)
	uint8_t unk_1a;			// 4			=>
	uint8_t unk_1b;			// 5			=> may be 2 uint4
	uint8_t unk_1c;			// 6			=>
	uint8_t unk_1d;			// 7			=> may be 2 uint4 
	uint32_t number_Section4_Next3_b; // 8	=> 0
	uint32_t offset_Section4_Next3_b; // C	=> to Section4_Next3_b
} PACKED;
static_assert(sizeof(FMP_Section4_Next3) == 0x10, "Incorrect structure size.");


struct FMP_Section4_Next3_b			//from Section4_Next3.unk1_offset
{
	uint32_t name_offset;		// 0			=> 
	uint8_t unk_1a;				// 4			=>
	uint8_t unk_1b;				// 4			=>
	uint8_t unk_1c;				// 4			=>
	uint8_t unk_1d;				// 4			=>
	uint32_t number_Section4_Next_f; // 8		=> old Next3_c is exactly like offset_Section4_Next_f
	uint32_t offset_Section4_Next_f; // C		=> to Section4_Next3_c
} PACKED;
static_assert(sizeof(FMP_Section4_Next3_b) == 0x10, "Incorrect structure size.");





struct FMP_Section4_Next2
{
	uint32_t offset_Section4_Next2_b; // 0		=> ref to FMP_Section4_Next2_b

	uint32_t unk_0;				// 4
	float transformMatrix4x3[12];	//=> m00, m01, m02 / m10, , m11, , m12 / m20, , m21, , m22 / m30, , m31, , m32 (translation) /

} PACKED;
static_assert(sizeof(FMP_Section4_Next2) == 0x38, "Incorrect structure size.");


struct FMP_Section4_Next2_b
{
	uint32_t name_offset;		// 0

	uint32_t unk_0;				// 4
	uint32_t number_Lod;		// 8		=> if 1, direct index for nsk and Emm (and float for LodDistance), else they are ref to a series of index (or floats)
	uint32_t indexNsk;			// C		=> ref to Dep1 (geometry). or Ref to a series of index ref to Dep1
	uint32_t indexTextureEmb;  // 10	=> ref to Dep2 (textures emb) ? 
	uint32_t indexEmm;			// 14	=> ref to Dep3 (emm).  or Ref to a series of index ref to Dep3
	uint32_t unk_5;				// 18
	uint32_t unk_6;				// 1C
	uint32_t indexEma;			// 20	=> ref to Dep4 (ema).
	uint32_t unk_8;				// 24
	float unk_9;				// 28		=> may be a widthX
	float unk_10;				// 2C		=> may be a widthY
	float lodDistance;			// 30		=> float or ref to series of float.
} PACKED;
static_assert(sizeof(FMP_Section4_Next2_b) == 0x34, "Incorrect structure size.");







struct FMP_Section4_Next4
{
	uint32_t number_Section4_Next4_c; // 0		=> count for Section4_Next4_c
	uint32_t number_Section4_Next4_b; // 4		=> count for Section4_Next4_b

	uint32_t offset_Section4_Next4_b; // 8		=> ref to Section4_Next4_b
	uint32_t offset_Section4_Next4_d; // C		=> ref to Section4_Next4_d
	uint32_t offset_Section4_Next4_c; // 10		=> ref to Section4_Next4_c
} PACKED;
static_assert(sizeof(FMP_Section4_Next4) == 0x14, "Incorrect structure size.");

struct FMP_Section4_Next4_b
{
	float unk_0;				// 0
	float unk_1;				// 4
	float unk_2;				// 8
	float unk_3;				// C
	float unk_4;				// 10
	float unk_5;				// 14
	float unk_6;				// 18
	float unk_7;				// 1C
	float unk_8;				// 20
	float unk_9;				// 24
	uint32_t unk_10;			// 28
	uint32_t indexToNext4_c_Transform;	// 2C
	
} PACKED;
static_assert(sizeof(FMP_Section4_Next4_b) == 0x30, "Incorrect structure size.");


struct FMP_Section4_Next4_c
{
	float positionX;			// 0
	float positionY;			// 4
	float positionZ;			// 8
	float rotationX;			// C	angle in radians Todo convert into degrees for Xml
	float rotationY;			// 10
	float rotationZ;			// 14	angle in radians
	float scaleX;				// 18	a lot of 1.0. for me 3 are a scale
	float scaleY;				// 1C	a lot as the same as unk6
	float scaleZ;				// 20	a lot as the same as unk6
} PACKED;
static_assert(sizeof(FMP_Section4_Next4_c) == 0x24, "Incorrect structure size.");




struct FMP_Section4_Next4_d
{
	uint8_t type;				// 0		=> if 0x2000 -> followed by Section4_Next4_e. if 1 is Indexes. if 0 it's a FMP_Section4_Next4_f
	uint8_t typeb;				// 		=> if 0x2000 -> followed by Section4_Next4_e. if 1 is Indexes. if 0 it's a FMP_Section4_Next4_f
	uint16_t number_Index;		// 2		=> number of followed Index (with padding to match uint32_t) => could be the same number as Next4_b ? todo check
	
} PACKED;
static_assert(sizeof(FMP_Section4_Next4_d) == 0x4, "Incorrect structure size.");


struct FMP_Section4_Next4_e					//could be recursive with Section4_Next4_d
{
	float unk_0;				// 0
	float unk_1;				// 4
	float unk_2;				// 8

	uint32_t unk0_offset;		// C	=> ref to a FMP_Section4_Next4_d
	uint32_t unk1_offset;		// 10	=> ref to a FMP_Section4_Next4_d
	uint32_t unk2_offset;		// 14	=> ref to a FMP_Section4_Next4_d
	uint32_t unk3_offset;		// 18	=> ref to a FMP_Section4_Next4_d
	uint32_t unk4_offset;		// 1C	=> ref to a FMP_Section4_Next4_d
	uint32_t unk5_offset;		// 20	=> ref to a FMP_Section4_Next4_d
	uint32_t unk6_offset;		// 24	=> ref to a FMP_Section4_Next4_d
	uint32_t unk7_offset;		// 28	=> ref to a FMP_Section4_Next4_d
} PACKED;
static_assert(sizeof(FMP_Section4_Next4_e) == 0x2C, "Incorrect structure size.");




















/////////////////////////////////////////////////////////////////////////////////




struct FMP_Section5
{
	uint32_t name_offset;				// 0	// [A] BFtok_colroot, SUN, sound_ENV, HYPNO_TARGET, HYPNO_OWN, SKY,  (sound like Section4's names, witch do the link. notice there is more names on Unk4Entry) 
	uint32_t number_Section5_Hitbox;	// 4
	uint32_t offset_Section5_Hitbox;	// 8
} PACKED;
static_assert(sizeof(FMP_Section5) == 0xC, "Incorrect structure size.");




struct FMP_Section5_Hitbox
{
	uint32_t name_offset;	// 0			=> BOX			[A] BOX, SPHERE, and a lot of names (sometime the same as Unk5Entry's names, but there is more name in this list).
	
	uint16_t child;			// 4			=> FF FF => null
	uint16_t unk_a0;		// 6			=> FF FF => null	[A] only 0 of 0xffff, may be a Bool  (may be for size)
	uint16_t sibling;		// 8			=> FF FF => null
	uint16_t parent;		// A			=> FF FF => null	[A] only 0 of 0xffff
	
	uint32_t number_Section5_Hitbox_b;	// E	=> [A] always 0
	uint32_t offset_Section5_Hitbox_b;	// 10	=> ref to Section5_Box_b

	uint32_t offset_Section5_Hitbox_f;	// 14 only in new version.

	uint32_t number_Section5_Hitbox_e;	// 18	=> 0 [A] value in [0, 0x53da], number of Section5_Box_e
	uint32_t offset_Section5_Hitbox_e;	// 1C	=> 0 => Ref to Section5_Box_e
	uint32_t number_Indexes;			// 20	=> 0 [A] count of series of uint16_t values (list of Id, may the one of each Section5_Box_e (same number)). if not null (and other unknowed cases), the next value is the offset_listIndex
	uint32_t offset_Indexes;			// 24

} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox) == 0x28, "Incorrect structure size.");


struct FMP_Section5_Hitbox_Old_V0x0x208XX	//Old version of the File
{
	uint32_t name_offset;	// 0			=> BOX			[A] BOX, SPHERE, and a lot of names (sometime the same as Unk5Entry's names, but there is more name in this list).

	uint16_t child;			// 4			=> FF FF => null
	uint16_t unk_a0;		// 6			=> FF FF => null	[A] only 0 of 0xffff, may be a Bool  (may be for size)
	uint16_t sibling;		// 8			=> FF FF => null
	uint16_t parent;		// A			=> FF FF => null	[A] only 0 of 0xffff

	uint32_t number_Section5_Hitbox_b;	// E	=> [A] always 0
	uint32_t offset_Section5_Hitbox_b;	// 10	=> ref to Section5_Box_b

	uint32_t number_Section5_Hitbox_e;	// 14	=> 0 [A] value in [0, 0x53da], number of Section5_Box_e
	uint32_t offset_Section5_Hitbox_e;	// 18	=> 0 => Ref to Section5_Box_e
	uint32_t number_Indexes;			// 1C	=> 0 [A] count of series of uint16_t values (list of Id, may the one of each Section5_Box_e (same number)). if not null (and other unknowed cases), the next value is the offset_listIndex
	uint32_t offset_Indexes;			// 20

} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox_Old_V0x0x208XX) == 0x24, "Incorrect structure size.");




struct FMP_Section5_Hitbox_b
{
	uint32_t number_Section5_Hitbox_c;	// 0			=> 0
	uint32_t offset_Section5_Hitbox_c;	// 4			=> ref to Section5_Box_c
} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox_b) == 0x8, "Incorrect structure size.");


struct FMP_Section5_Hitbox_c
{
	uint32_t unk_0;			// 0			=> 0
	uint32_t offset_Section5_Hitbox_d;	// 4			=> ref to a Section5_Box_d
	uint32_t unk_1;			// 8			=> 0 [A] always 0
	uint32_t unk_2;			// C			=> 0 [A] always 0
	uint32_t size_Section5_File; // 10		=> 0 size in octets of part started by unk3_offset (seam to be a file inserted)
	uint32_t offset_Section5_File; // 14	=> 0 start of a part (seam to be a file inserted)
	uint32_t offset_Section5_Hitbox_d_2; // 18	=> ref to a Section5_Box_d
	uint32_t unk_5;			// 1C			=> 0 [A] always 0
	uint32_t unk_6;			// 20			=> 0 [A] only 0 or 0x3c23d70a
	float unk_7;			// 24			=> 0A D7 23 3C ... ( 0.0099999997764826 si float ). [A] only 0, 1, 2 or 0x3c23d70a

} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox_c) == 0x28, "Incorrect structure size.");



struct FMP_Section5_Hitbox_d
{
	uint32_t unk_0;		// 0			=> 1	[A] only 0, 1, 2, 3 or 0xb6800000
	float unk_1;		// 4			=> 0
	float unk_2;		// 8			=> 0
	float unk_3;		// C			=> 0
	float yaw;			// 10			=> 0
	float pitch;		// 14			=> -0.0 (generalement les 00 00 00 80 correspondent a des float -0.0)
	float roll;			// 18			=> 0
	float unk_7;		// 1C			=> 00 00 00 3F (0.5 si float)
	float unk_8;		// 20			=> 00 00 00 3F (0.5 si float)
	float unk_9;		// 24			=> 00 00 00 3F (0.5 si float)
	float unk_10;		// 28			=> 0 [A] always 0
	float unk_11;		// 2C			=> 0 [A] always 0
	float unk_12;		// 30			=> 0
	float unk_13;		// 34			=> 0 [A] only 0, 1, 2, 0x3054414d

} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox_d) == 0x38, "Incorrect structure size.");




struct FMP_Section5_Hitbox_e
{
	float unk_0;		// 0			=> 
	float unk_1;		// 4			=> 
	float unk_2;		// 8			=> 
	float unk_3;		// C			=> 
	float unk_4;		// 10			=> 
	float unk_5;		// 14			=> 

} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox_e) == 0x18, "Incorrect structure size.");



struct FMP_Section5_Hitbox_f
{
	uint32_t unk_0;			// 0			=> 0
	uint32_t size_Section5_File;			// 4			=> 0
	uint32_t offset_Section5_File;	// 8			=> ref to a Section5_Box_d
	
} PACKED;
static_assert(sizeof(FMP_Section5_Hitbox_f) == 0xC, "Incorrect structure size.");




////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



/* ------ Resume:

-Section0a: lot of floats values. could have:
			-the stage dimensions.
			-the origine
			-the start player
-Section0b: 
	-lots of unknow values witch are not offsets
	-lot of TypeName (51 = 0x33): "Ground", "Wall", "InvisibleWall", "BgObject",  "Chara", "NPC", "WaterSurface" witch may be used by index somewhere.

-Section1: the name suggest a start of a Script/Scenario/Action : "TRIGGER_CONTACT_SOLVER", "Default", "__118"
-Section2: could be the same : a start of a Script/Scenario/Action : "Default", "__105"


-Section3: entities witch group some Section4. (may be only fgor instanced, or may be only for script manipulation)
		notice : all Section4 are not used by the Section3. That strange.

-Section4: "bnSKY", "bnBFten01", "LCT00_obj_BFten02", "BFten03"
	-TransformMatrix.
	(notice: the same Next could be called by many Section4. same for Next2, Next3 and Next4)
	-index of Section5
	-listNext (number depend of number of Hitbox on Linked Section5)
		-Next: 
			-list of Next_b: couple of index (4 couple max)
			-Next_c: lot of floats. there is a width (3 dims), and a quaternion
			-a second Next_c: lot of floats. there is a width (3 dims), and a quaternion
			-Next_d:
				-list of Next_e:
					-name: PLAY SOUND 3D
					-list of Next_f: it's parameters with differents type of value: 
						-name like "Sound ID", "Volume", 
						-value: could be vector4 for direction or position, and some value are "@self"
			-TransformMatrix.

	-Next3:			=> seam to play with scripts ? Sound as "Actions"
		-list Next3_b:  Sound a "Action"
			-name: "MOTION SET", "SET LCT","Small Scene" (with "Movie Name").  sound "Function"
			-list of Next_f: parameters, "Object Name", "Motion ID" (may be reference to Ema), "Start Frame", 

	-List_Next2:
		-Next2:	"SceneNode" (because of Transform)
			-Next2_b: "Entity"
				-name: "BFten00", "BFten02", it name of nsk ? no because of "BFten_LCT", but it's look similar because it's here there is lod of objects
				-type is in reality "NumberLOD"
					if(NumberLOD == 1)
					{
						-unk2 = dep1 = nsk index
						-unk4 = dep3 = emm index
						-unk11 = float => distance validity LOD
						
						-unk3 = dep2 = emb index
						-unk7 = dep4 = ema index, 0xffffffff = -1 for none.
					} else {
						-unk2 = refTo NumberLOD x uint32
						-unk4 = refTo NumberLOD x uint32
						-unk11 = refTo NumberLOD x float => that give LOD range validity
					}
			-TransformMatrix.
		
	-Next4:
		-list Next4_b: 
			-list of float
			-index of Next4_c
		-list Next4_c: position +  rotation + scale.
		-Next4_d:
			-type
			if(type==1)				//may be for the last child of the hierarchy , for add index of "SceneNode" or "Next"
			{
				-list d'index (16bits). 
					-but there is stranges values too high :
						-there is also the type, like it's another Next4_d, sound strange (could be a "lastIndex" instead of "numberOf" ?).
						-values sound like semi offset for something else.
			}else{
				-Next4_e
					-3 floats (position ?) + pointer for recursive on Next4_d. that make a Hierarchy.
			}

-Section5: 
	-name: sound like Section4's names
	-List HitBox:
		-HitBox: 
			-name: BOX, SPHERE, 00_ivent_010, ACT19_04, AI_MOVE_03, A_Env_Sound_32_1, BFice_StoneA_col, QUEST_1001_POS_00
			-may child + sibling, parent index.
			-list Hitbox_b:
				-Hitbox_b:
					-list Hitbox_c:
						-Hitbox_c:
							-Hitbox_d: lots of floats, may be there is yaw pitch and roll inside.
							-a Havok File, certainly for Destruction animation. So near to that part, should have the condition for script play the destruction on hit of the hitbox. 
							-a second Hitbox_d: may be linked on the second Next_c ?
			-Hitbox_f:	(this only in the last version of the file, may be because SDK 2015 of Havok seam to have more fonctionnalities, some could animate many objects, instead of version 2014, in Hitbox_c, with on by destructed object parts.)
				-there is a unk_0 always at 1, may be it's a number of Havok file. but the game crash if I change it to 0 or 2, but it's could be normal, depend of what expected the scripts.
				-a Havok File, certainly for Destruction animation. So near to that part, should have the condition for script play the destruction on hit of the hitbox.
			-list Hitbox_e: lots of Hitbox_e . sound like a mesh definition for custom hitbox by polygone. (it's a operation after the classical Hitbox detection)
				-Hitbox_e: lot of floats. it's look like a position + a direction (Normal ?).
			-list Index: there is a very lot of values. may be connected to Hitbox_e. There is series of 3 values, sound like faces index for a mesh definition. 
*/




////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



struct FMP_Parameter									//Section4_Next_f
{
	enum TypeValue
	{
		TV_Bool = 0,
		TV_Int,
		TV_Float,
		TV_Direction,
		TV_Position,
		TV_String = 6,
		TV_Uint = 8
	};

	string name;
	TypeValue typeValue;

	bool boolValue;
	int intValue;
	float floatValue;
	float directionValue[4];
	float positionValue[4];
	string stringValue;
	uint32_t uintValue;

	bool operator==(const FMP_Parameter& other);
	bool operator!=(const FMP_Parameter& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml();
	bool import_Xml(TiXmlElement* parent);

} PACKED;
static_assert(sizeof(FMP_Section4_Next_f) == 0xC, "Incorrect structure size.");



class FMP_Action										//Section4_Next3
{
friend class FmpFile;

	struct Command										//Section4_Next3_b
	{
		string functionName;
		size_t unk_1a;
		size_t unk_1b;
		size_t unk_1c;
		size_t unk_1d;
		std::vector<FMP_Parameter> parameters;			//Section4_Next_f

		Command(string name) { functionName = name; unk_1a = 3; unk_1b = 0, unk_1c = 0; unk_1d = 0; };
		bool operator==(const Command& other);
		bool operator!=(const Command& other) { return (!(*this == other)); }
		
		TiXmlElement* export_Xml(bool withComments = false);
		bool import_Xml(TiXmlElement* parent);
	};
	
	size_t unk_0;
	size_t unk_1a;
	size_t unk_1b;
	size_t unk_1c;
	size_t unk_1d;

	std::vector<Command> listCommands;

	//Notice: here seam to be the best place for a link with S1, S2 or typeName in s0b.

public:
	FMP_Action() { Reset(); }
	virtual ~FMP_Action() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next3);
	void Reset();
	FMP_Action* clone() { FMP_Action* action = new FMP_Action(); action->unk_0 = unk_0; action->unk_1a = unk_1a; action->unk_1b = unk_1b; action->unk_1c = unk_1c; action->unk_1d = unk_1d; action->listCommands = listCommands; return action; }
	bool operator==(const FMP_Action& other);
	bool operator!=(const FMP_Action& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};


/////////////////////////////////////////////////////////////////////////////////////


class Havok_File								//Section5_File
{
friend class FmpFile;

	uint8_t* buffer;
	size_t size;
	
public:
	Havok_File() { buffer = 0;  Reset(); }
	Havok_File(Havok_File* other) { buffer = 0;  Reset(); if ((other->buffer) && (other->size)) { buffer = (uint8_t*)malloc(other->size); memcpy(buffer, other->buffer, other->size); size = other->size; } }
	virtual ~Havok_File() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section5_File, size_t size_Section5_File);
	void Reset();
	void operator=(Havok_File* other) { Reset(); if ((other->buffer) && (other->size)) { buffer = (uint8_t*)malloc(other->size); memcpy(buffer, other->buffer, other->size); size = other->size; } }
	void operator=(Havok_File& other) { Reset(); if ((other.buffer) && (other.size)) { buffer = (uint8_t*)malloc(other.size); memcpy(buffer, other.buffer, other.size); size = other.size; } }
	Havok_File* clone() { Havok_File* file = new Havok_File(); if ((buffer) && (size)) { file->buffer = (uint8_t*)malloc(size); memcpy(file->buffer, buffer, size); file->size = size; } return file; }

	TiXmlElement* export_Xml(string filename);
	bool import_Xml(TiXmlElement* parent, string originefilename);
};







class FMP_Destruction_sub								//Section5_Hitbox_d
{
friend class FmpFile;

	size_t unk_0;
	float unk_1;
	float unk_2;
	float unk_3;
	float yaw;											//in degrees
	float pitch;
	float roll;
	float unk_7;
	float unk_8;
	float unk_9;
	float unk_10;
	float unk_11;
	float unk_12;
	float unk_13;

public:
	FMP_Destruction_sub() { Reset(); }
	virtual ~FMP_Destruction_sub() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section5_Hitbox_d);
	void Reset();
	FMP_Destruction_sub* clone(){ FMP_Destruction_sub* sub = new FMP_Destruction_sub(); sub->unk_0 = unk_0; sub->unk_1 = unk_1; sub->unk_2 = unk_2; sub->unk_3 = unk_3; sub->yaw = yaw; sub->pitch = pitch; sub->roll = roll; sub->unk_7 = unk_7; sub->unk_8 = unk_8; sub->unk_9 = unk_9; sub->unk_10 = unk_10; sub->unk_11 = unk_11; sub->unk_12 = unk_12; sub->unk_13 = unk_13; return sub; }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};




class FMP_Destruction								//Section5_Hitbox_c
{
friend class FmpFile;

	size_t unk_0;
	size_t unk_1;
	size_t unk_2;
	size_t unk_5;
	size_t unk_6;
	float unk_7;

	bool haveSub0;
	FMP_Destruction_sub sub0;
	bool haveSub1;
	FMP_Destruction_sub sub1;
	
	size_t havokFileIndex;

public:
	FMP_Destruction() { Reset(); }
	virtual ~FMP_Destruction() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section5_Hitbox_c, std::vector<Havok_File*> &mListHavokFile);
	void Reset();

	TiXmlElement* export_Xml(string filename, size_t indexGroup, size_t indexDestructionGroup, size_t indexDestruction, std::vector<Havok_File*> &listHavokFile, bool withComments = false);
	bool import_Xml(TiXmlElement* parent, std::vector<Havok_File*> &listHavokFile, string filename);
};




class FMP_Hitbox						//Section5_Hitbox
{
friend class FmpFile;

	struct Vertex						//Section5_Hitbox_e
	{
		float position[3];
		float normal[3];

		Vertex(float posX, float posY, float posZ, float normalX, float normalY, float normalZ) { position[0] = posX; position[1] = posY; position[2] = posZ; normal[0] = normalX; normal[1] = normalY; normal[2] = normalZ; }
	};
	
	
	string name;
	
	size_t child;						//Todo check hierarchy and think about it  if it's a Yes.
	size_t unk_a0;
	size_t sibling;
	size_t parent;

	std::vector<std::vector<FMP_Destruction>> listOnDestruction;	//Section5_Hitbox_b
	std::vector<size_t> listHavokFilesIndex;				// Section5_Hitbox_f   hyp: all file have the same size (if there is more than one file)


	//definition of a geometry for colisions
	std::vector<Vertex> listVertex;
	std::vector<size_t> listFaceIndex;

	size_t debug_startOffset_vertex;
	size_t debug_startOffset_faces;

public:
	FMP_Hitbox() { Reset(); }
	virtual ~FMP_Hitbox() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, FMP_Section5_Hitbox* section5_hitbox, std::vector<Havok_File*> &mListHavokFile);
	void Reset();

	TiXmlElement* export_Xml(string filename, std::vector<Havok_File*> &listHavokFile, size_t indexGroup, bool withComments = false);
	bool import_Xml(TiXmlElement* parent, std::vector<Havok_File*> &listHavokFile, string filename);
};



class FMP_HitboxGroup					//Section5
{
friend class FmpFile;

	string name;

	std::vector<FMP_Hitbox> listHitbox;

public:
	FMP_HitboxGroup() { Reset(); }
	virtual ~FMP_HitboxGroup() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_section5, size_t version, std::vector<Havok_File*> &mListHavokFile);
	void Reset();

	TiXmlElement* export_Xml(string filename, std::vector<Havok_File*> &listHavokFile);
	bool import_Xml(TiXmlElement* parent, std::vector<Havok_File*> &listHavokFile, string filename);
};









/////////////////////////////////////////////////////////////////////////////////////


class FMP_VirtualSubPart_sub						//Section4_Next_c
{
friend class FmpFile;

	size_t unk_0;
	size_t unk_0b;
	size_t unk_1;
	size_t unk_1b;
	size_t unk_2;
	float unk_3;
	float unk_4;
	float unk_5;
	float unk_6;
	float unk_7;
	float unk_8;
	float unk_9;
	float unk_10;
	float unk_11;
	float unk_12;
	float unk_13;
	float widthX;
	float widthY;
	float widthZ;
	float quaternionX;
	float quaternionY;
	float quaternionZ;
	float quaternionW;

public:
	FMP_VirtualSubPart_sub() { Reset(); }
	virtual ~FMP_VirtualSubPart_sub() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next_c);
	void Reset();
	FMP_VirtualSubPart_sub* clone() { FMP_VirtualSubPart_sub* sub = new FMP_VirtualSubPart_sub(); sub->unk_0 = unk_0; sub->unk_0b = unk_0b; sub->unk_1 = unk_1; sub->unk_1b = unk_1b; sub->unk_2 = unk_2; sub->unk_3 = unk_3; sub->unk_4 = unk_4; sub->unk_5 = unk_5; sub->unk_6 = unk_6; sub->unk_7 = unk_7; sub->unk_8 = unk_8; sub->unk_9 = unk_9; sub->unk_10 = unk_10; sub->unk_11 = unk_11; sub->unk_12 = unk_12; sub->unk_13 = unk_13; sub->widthX = widthX; sub->widthY = widthY; sub->widthZ = widthZ; sub->quaternionX = quaternionX; sub->quaternionY = quaternionY; sub->quaternionZ = quaternionZ; sub->quaternionW = quaternionW; return sub; }
	bool operator==(const FMP_VirtualSubPart_sub& other);
	bool operator!=(const FMP_VirtualSubPart_sub& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};




class FMP_VirtualSubPart						//Section4_Next. may be we call rename "Event". Todo see.
{
friend class FmpFile;

	std::vector<std::vector<size_t>> listIndex;	//Section4_Next_b. (hyp always 2 value by group). todo find what is for. may be a link inside Hitbox ?

	FMP_VirtualSubPart_sub* sub0;				//Section4_Next_c
	FMP_VirtualSubPart_sub* sub1;
	FMP_Action* action;							//Section4_Next_d = Section4_Next3

	size_t unk_2;
	size_t unk_2b;
	float unk_4;
	float unk_5;

	float transformMatrix4x3[12];
	//float position[3];		//TODo instead of transformMatrix4x3
	//float rotation[3];
	//float scale[3];

public:
	FMP_VirtualSubPart(void) { sub0 = 0; sub1 = 0;  action = 0;  Reset(); }
	virtual ~FMP_VirtualSubPart() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next);
	void Reset();
	bool operator==(const FMP_VirtualSubPart& other);
	bool operator!=(const FMP_VirtualSubPart& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};



/////////////////////////////////////////////////////////////////////////////////////


class FMP_Entity						//Section4_Next2 + Section4_Next2_b
{
friend class FmpFile;

	struct Lod
	{
		string nsk_filename;
		string emm_filename;
		float distance;
		
		Lod(string nsk_filename, string emm_filename, float distance) { this->nsk_filename = nsk_filename; this->emm_filename = emm_filename; this->distance = distance; };
	};
	
	size_t s4n2_unk_0;
	float transformMatrix4x3[12];
	//float position[3];		//TODo instead of transformMatrix4x3
	//float rotation[3];
	//float scale[3];

	
	
	bool haveVisualPart;					//for consider or not the next parameters (Section4_Next2_b)
	string name;

	std::vector<Lod> listLod;				//Notice: (if haveVisualPart) there is one lod minimum.
	string embTexture_filename;				//all lod share Emb and Ema
	string ema_filename;

	size_t s4n2b_unk_0;
	size_t s4n2b_unk_5;
	size_t s4n2b_unk_6;
	size_t s4n2b_unk_8;
	float s4n2b_unk_9;
	float s4n2b_unk_10;
	
public:
	FMP_Entity() { Reset(); }
	virtual ~FMP_Entity() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next2, std::vector<string> &listNskFile, std::vector<string> &listTextureEmbFile, std::vector<string> &listEmmFile, std::vector<string> &listEmaFile);
	void Reset();
	bool operator==(const FMP_Entity& other);
	bool operator!=(const FMP_Entity& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};



/////////////////////////////////////////////////////////////////////////////////////



class FMP_HierarchyNode						//Section4_Next4_d	+ 	Section4_Next4_e
{
friend class FmpFile;

	size_t typeb;
	float unk_0;							//may be for position
	float unk_1;
	float unk_2;

	std::vector<FMP_HierarchyNode*> listChild;		//hyp: there is Child OR index on each node, not both in the same time.
													//notice: it could have null because it could have child after

	std::vector<size_t> listIndex;				//Todo merge with Node after confirm index are for Node: confirmed for normal index, but there is some 0x2000, 0x3000, 0x6000 and highest strange values. So keep it separate, for now.


public:
	FMP_HierarchyNode() { Reset(); }
	virtual ~FMP_HierarchyNode() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4_d);
	void Reset();
	bool operator==(const FMP_HierarchyNode& other);
	bool operator!=(const FMP_HierarchyNode& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};




class FMP_NodeTransform							//Section4_Next4_c
{
friend class FmpFile;

	float position[3];
	float rotation[3];
	float scale[3];

public:
	FMP_NodeTransform() { Reset(); }
	virtual ~FMP_NodeTransform() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4_c);
	void Reset();
	bool operator==(const FMP_NodeTransform& other) { return ( (position[0] == other.position[0])&& (position[1] == other.position[1]) &&  (position[2] == other.position[2]) &&  (rotation[0] == other.rotation[0]) && (rotation[1] == other.rotation[1]) && (rotation[2] == other.rotation[2]) && (scale[0] == other.scale[0]) && (scale[1] == other.scale[1]) && (scale[2] == other.scale[2]) ); }
	bool operator!=(const FMP_NodeTransform& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml();
	bool import_Xml(TiXmlElement* parent);
};

class FMP_Node							//Section4_Next4_b	+ Section4_Next4_c
{
friend class FmpFile;

	float unk_0;
	float unk_1;
	float unk_2;
	float unk_3;
	float unk_4;
	float unk_5;
	float unk_6;
	float unk_7;
	float unk_8;
	float unk_9;
	size_t unk_10;	
	size_t transformIndex;				// traget a FMP_NodeTransform

public:
	FMP_Node() { Reset(); }
	virtual ~FMP_Node() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4_b);
	void Reset();
	bool operator==(const FMP_Node& other) { return ((unk_0 == other.unk_0)&& (unk_1 == other.unk_1) && (unk_2 == other.unk_2) && (unk_3 == other.unk_3) && (unk_4 == other.unk_4) && (unk_5 == other.unk_5) && (unk_6 == other.unk_6) && (unk_7 == other.unk_7) && (unk_8 == other.unk_8) && (unk_9 == other.unk_9) && (unk_10 == other.unk_10) && (transformIndex == other.transformIndex) ); }
	bool operator!=(const FMP_Node& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};



class FMP_Hierarchy						//Section4_Next4
{
friend class FmpFile;

	std::vector<FMP_Node> listNode;
	std::vector<FMP_NodeTransform> listTransform;
	FMP_HierarchyNode* mRoot;			//recursive.

public:
	FMP_Hierarchy() { mRoot = 0;  Reset(); }
	virtual ~FMP_Hierarchy() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4);
	void Reset();
	bool operator==(const FMP_Hierarchy& other);
	bool operator!=(const FMP_Hierarchy& other) { return (!(*this == other)); }

	TiXmlElement* export_Xml();
	bool import_Xml(TiXmlElement* parent);
};





/////////////////////////////////////////////////////////////////////////////////////


class FMP_Object						//Section4
{
friend class FmpFile;

	string name;
	size_t hitboxGroupIndex;			//section5


	size_t unk_04;
	size_t unk_04b;
	size_t unk_0A;
	size_t unk_0B;
	float unk_20;
	
	float transformMatrix4x3[12];
	//float position[3];		//TODo instead of transformMatrix4x3
	//float rotation[3];
	//float scale[3];

	std::vector<FMP_Entity> listEntity;
	std::vector<FMP_VirtualSubPart*> listVirtualSubParts;			//hyp : all have a Hitbox (because the number of Next depend of number of )
	FMP_Action* action;
	FMP_Hierarchy* hierarchy;

public:
	FMP_Object() { action = 0; hierarchy = 0;  Reset(); }
	virtual ~FMP_Object() { Reset(); }

	bool Load(const uint8_t *buf, size_t size, size_t offset_Section4, size_t nb_Section4_Next, std::vector<string> &listNskFile, std::vector<string> &listTextureEmbFile, std::vector<string> &listEmmFile, std::vector<string> &listEmaFile);
	void Reset();
	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};






/////////////////////////////////////////////////////////////////////////////////////






class FMP_S1
{
friend class FmpFile;

	string name;

	size_t unk_04;
	float unk_08;
	float unk_0C;
	float unk_10;
	float unk_14;

public:
	FMP_S1() { Reset(); }
	virtual ~FMP_S1() { Reset(); }

	void Reset();
	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};



class FMP_S2
{
friend class FmpFile;

	string name;

	float unk_04;
	float unk_08;
	float unk_0C;
	float unk_10;
	float unk_14;
	float unk_18;

public:
	FMP_S2() { Reset(); }
	virtual ~FMP_S2() { Reset(); }

	void Reset();
	TiXmlElement* export_Xml(bool withComments = false);
	bool import_Xml(TiXmlElement* parent);
};




#ifdef _MSC_VER
#pragma pack(pop)
#endif


class FmpFile : public LibXenoverse::EMO_BaseFile
{
public:
	string originefilename;					// to solve some trouble.
	
private:
	//section Section0a:
	float width_X;
	float width_Y;
	float width_Z;

	float s0a_unk_3;
	float s0a_unk_4;
	float s0a_unk_5;

	float s0a_unk_6;
	size_t s0a_unk_7;

	float s0a_unk_8;
	float s0a_unk_9;
	float s0a_unk_10;

	float s0a_unk_11;
	float s0a_unk_12;
	float s0a_unk_13;

	float s0a_unk_14;
	float s0a_unk_15;
	float s0a_unk_16;

	float s0a_unk_17;
	float s0a_unk_18;

	float s0a_unk_19;
	float s0a_unk_20;
	float s0a_unk_21;

	float s0a_unk_22;
	float s0a_unk_23;
	float s0a_unk_24;

	float s0a_unk_25;
	float s0a_unk_26;
	float s0a_unk_27;
	float s0a_unk_28;

	float s0a_unk_29;
	float s0a_unk_30;
	float s0a_unk_31;

	float nearDistance;
	float farDistance;
	float s0a_unk_34;



	//section Section0b:
	size_t s0b_unk_0;
	size_t s0b_unk_1;
	size_t s0b_unk_2;
	size_t s0b_unk_3;
	size_t s0b_unk_4;
	size_t s0b_unk_5;
	size_t s0b_unk_6;
	size_t s0b_unk_7;
	size_t s0b_unk_8;
	size_t s0b_unk_9;
	size_t s0b_unk_10;
	size_t s0b_unk_11;
	size_t s0b_unk_12;
	size_t s0b_unk_13;
	size_t s0b_unk_14;
	size_t s0b_unk_15;
	size_t s0b_unk_16;
	size_t s0b_unk_17;
	size_t s0b_unk_18;
	size_t s0b_unk_19;
	size_t s0b_unk_20;
	size_t s0b_unk_21;
	size_t s0b_unk_22;
	size_t s0b_unk_23;
	size_t s0b_unk_23b;
	size_t s0b_unk_24;
	size_t s0b_unk_25;
	size_t s0b_unk_26;
	size_t s0b_unk_27;
	size_t s0b_unk_28;
	size_t s0b_unk_29;
	size_t s0b_unk_30;
	size_t s0b_unk_31;
	size_t s0b_unk_32;
	size_t s0b_unk_33;
	size_t s0b_unk_34;
	size_t s0b_unk_35;
	size_t s0b_unk_36;
	size_t s0b_unk_37;
	size_t s0b_unk_38;
	size_t s0b_unk_39;
	size_t s0b_unk_40;
	size_t s0b_unk_41;
	size_t s0b_unk_42;
	size_t s0b_unk_43;
	size_t s0b_unk_44;
	size_t s0b_unk_45;
	size_t s0b_unk_46;
	size_t s0b_unk_47;
	size_t s0b_unk_48;
	size_t s0b_unk_49;
	size_t s0b_unk_50;
	size_t s0b_unk_51;
	size_t s0b_unk_52;
	size_t s0b_unk_53;
	size_t s0b_unk_54;

	std::vector<string> mListTypeName;




	std::vector<FMP_S1> mListS1;
	std::vector<FMP_S2> mListS2;

	std::vector<FMP_Object*> mListObject;		//section4
	std::vector<FMP_HitboxGroup> mListHitboxGroup;	//section5


	struct FragmentsGroup						//Section3
	{
		string name;
		std::vector<size_t> listObjects;

		FragmentsGroup(string name) { this->name = name; }
	};
	std::vector<FragmentsGroup> mListFragmentsGroups;


	std::vector<Havok_File*> mListHavokFile;
	std::vector<string> listNsk;
	std::vector<string> listTextureEmb;
	std::vector<string> listEmm;
	std::vector<string> listEma;



	//Debug/work:
	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.
	size_t lastSection;

public:
	FmpFile();
	virtual ~FmpFile() { Reset(); }

	void Reset();
	virtual bool load(string filename) { return this->SmartLoad(filename); }
	bool Load(const uint8_t *buf, size_t size);
	virtual uint8_t *CreateFile(unsigned int *psize);
	size_t write(uint8_t *buf, size_t size);
	size_t calculeFilesize();


	void saveXml(string filename);
	TiXmlElement* export_Xml(string filename);
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false);
	bool import_Xml(TiXmlElement* parent);



	void save_Xml(string filename, bool show_error = false);		// Xml version for debug.
	void write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size, string filename);
	
	void save_Coloration(string filename, bool show_error = false);		//create a file for wxHexEditor, for add tag and color on section of the file.
	void write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size);
	void write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList = (size_t)-1, bool checkAllreadyTaggued = true);
	void write_Coloration_Gaps(size_t startOffsetToConsidere, size_t endOffsetToConsidere, size_t sizeFilter, size_t start_idTag, string sectionName, TiXmlElement* parent, std::vector<bool> &listBytesAllreadyTagged);
};





}

#endif // FMPFILE_H
