#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <KVDBAPI.h>
#include <Utils/Bstream.h>
#include <Utils/VarInt.h>
#include <MC/ServerPlayer.hpp>
#include <MC/Level.hpp>
#include <GuiAPI.h>
#include <RegCommandAPI.h>
#include <ScheduleAPI.h>
#include <SendPacketAPI.h>
using namespace std;
std::unique_ptr<KVDB> NPCdb;
typedef unsigned long long eid_t;
static eid_t NPCID = 922337203684324209ull;
using namespace RegisterCommandHelper;
void broadcastNPCREMOVE(eid_t nid);
void broadcastNPCADD(eid_t nid);

string MINECRAFT_ENTITY_TYPE(string x) {
	if (x.find(':') != x.npos) return x;
	else return "minecraft:" + x;
}

string& replace_all_distinct(string& str, const   string& old_value, const   string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}



constexpr unsigned int H(string_view s)
{
	auto str = s.data();
	unsigned int hash = 5381;
	while (*str) {
		hash = ((hash << 5) + hash) + (*str++); /* times 33 */
	}
	hash &= ~(1 << 31); /* strip the highest bit */
	return hash;
}

struct NPC {
	string name;
	string nametag;
	string type;
	Vec3 pos;
	Vec3 rot;
	eid_t eid;
	string data;
	float size;
	int dim;
#define SBIT(x) (1ull<<x)
	unsigned long long flag = SBIT(14) | SBIT(15);
	void pack(WBStream& bs)const {
		bs.apply(name, nametag, type, pos, rot, eid, data, size);
	}
	static unsigned long long getFLAGS(string_view val) {
		split_view spv(val);
		unsigned long long rv = 0;
		for (; !spv.end(); ++spv) {
			int bit = atoi(spv.get());
			rv |= 1ull << bit;
		}
		return rv;
	}
	void unpack(RBStream& bs) {
		bs.apply(name, nametag, type, pos, rot, eid, data, size);
		type = MINECRAFT_ENTITY_TYPE(type);
		string val;
		if (NPCdb->get(string("npcdata_") + name, val)) {
			flag = getFLAGS(val);
		}
		else {
			flag = SBIT(14) | SBIT(15);
		}
		if (NPCdb->get(string("npcdim_") + name, val)) {
			dim = *(int*)val.data();
		}
		else {
			dim = 0;
		}
	}
	void NetAdd(WBStream& bs)const {
		bs.apply(VarULong(eid), VarULong(eid), MCString(type), pos, Vec3{ 0,0,0 }
			, rot, //rotation
			VarUInt(0), //attr
			VarUInt(4), //metadata :4
			VarUInt(0), VarUInt(7), VarULong(flag), //FLAGS:showtag
			VarUInt(81), VarUInt(0), (char)1, //always show tag
			VarUInt(4), VarUInt(4), MCString(nametag), //nametag
			VarUInt(0x26), VarUInt(3), (float)size, // 
			VarUInt(0) //entity link
		);
	}
	void NetRemove(WBStream& bs) const {
		bs.apply(VarULong(eid));
	}
};

bool isNum(string str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

int getNewDimId(string a1) {
	switch (H(a1))
	{
	case H("主世界"):
		return 0;
		break;
	case H("下届"):
		return 1;
		break;
	case H("末地"):
		return 2;
		break;
	default:
		break;
	}
	return -1;
}

std::unordered_map<eid_t, NPC> npcs;

void broadcastPKT(Packet& pk, int dimid) {
	auto usr = Level::getAllPlayers();
	for (auto i : usr) {
		if (i->getDimensionId() == dimid) {
			auto spp = (ServerPlayer*)i;
			spp->sendNetworkPacket(pk);
		}
	}
}
void broadcastNPCADD(eid_t nid) {
	WBStream ws;
	npcs[nid].NetAdd(ws);
	NetworkPacket<0xd> pk{ ws };
	broadcastPKT(pk, npcs[nid].dim);
}

void broadcastNPCREMOVE(eid_t nid) {
	WBStream ws;
	npcs[nid].NetRemove(ws);
	NetworkPacket <14)> pk{ ws };
	broadcastPKT(pk, npcs[nid].dim);
}
void __sendADDNPC(ServerPlayer& wp, NPC const& n) {
	WBStream ws;
	n.NetAdd(ws);
	NetworkPacket<0xd> pk{ ws };
	wp.sendNetworkPacket(pk);
}
void __sendDELNPC(ServerPlayer& wp, NPC const& n) {
	WBStream ws;
	n.NetRemove(ws);
	NetworkPacket<14> pk{ ws };
	wp.sendNetworkPacket(pk);
}
void resendAllNPC(ServerPlayer& wp) {
	auto dimid = wp.getDimensionId();
	for (auto const& [id, n] : npcs) {
		if (n.dim == dimid) {
			__sendADDNPC(wp, n);
		}
		else {
			__sendDELNPC(wp, n);
		}
	}
}


class Vec2 {
public:
	float x, z;
};

bool iszh(ServerPlayer* a1) {
	for (auto a : langs) {
		if (offPlayer::getRealName(a1) == a.first) {
			if (a.second == "zh_CN") {
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	return 0;
}

inline static ServerPlayer* MakeSP(CommandOrigin const& ori) {
	ServerPlayer* sp = (ServerPlayer*)ori.getEntity();
	if (sp) {
		return { sp };
	}
	return nullptr;
}

class NPCCommand : public Command {

	enum class NPCType {
		LIST = 1,
		GUI = 2,
		ADD = 3,
		SETROT = 4,
		REMOVE= 5,
		DFLAG = 6
	} operation;


	string addname, addtype, addtag, adddata;
	string listoptional;
	bool listoptional_iset;
	string setrot;
	string remove;
	string dflagname, flag;
public:
	void execute(CommandOrigin const& ori, CommandOutput& outp) const override {
		switch (operation)
		{
		case NPCCommand::NPCType::LIST:
			for (auto& i : npcs) {
				if (listoptional_iset) {
					if (i.second.name == listoptional) {
						outp.addMessage("NPC " + i.second.name + " 名称 " + i.second.nametag + " 坐标 " + i.second.pos.toString() + " 维度 " + std::to_string(i.second.dim));
					}
				}
				else {
					outp.addMessage("NPC " + i.second.name + " 名称 " + i.second.nametag + " 坐标 " + i.second.pos.toString() + " 维度 " + std::to_string(i.second.dim));
				}
			break;
		case NPCCommand::NPCType::GUI:
			ori.
			using namespace GUI;
			auto npcgui = make_shared<SimpleForm>();
			if (iszh(MakeSP(ori))) {
				npcgui->title = u8"§l§eNPC系统";
				npcgui->content = u8"HI " + pl->getNameTag();
				npcgui->reset();
				npcgui->addButton(GUIButton(u8"§l§g添加NPC"));
				npcgui->addButton(GUIButton(u8"§l§c删除NPC"));
				npcgui->addButton(GUIButton(u8"§l§6NPC列表"));
			}
			sendForm(wp1, SimpleFormBinder(npcgui, [&](ServerPlayer& wp2, SimpleFormBinder::DType data1) {
				if (data1.set) {
					if (data1.val().first == 0) {
						auto fm = std::make_shared<FullForm>();
						if (iszh(wp2)) {
							fm->title = u8"§l§e请设置NPC";
							fm->addWidget({ GUILabel("HI " + wp2->getNameTag()) });
							fm->addWidget({ GUIInput(u8"§l§beNPCID（不可重复）",u8"例：npc123") });
							fm->addWidget({ GUIInput(u8"§l§bNPC类型（例:pig,cow）",u8"例：pig") });
							fm->addWidget({ GUIInput(u8"§l§bNPC标签（显示的名称）",u8"例：我是商人") });
							fm->addWidget({ GUIInput(u8"§l§b大小",u8"例：1.5") });
							fm->addWidget({ GUIInput(u8"§l§b触发指令",u8"例：list") });
						}
						else {
							fm->title = u8"§l§e请设置NPC";
							fm->addWidget({ GUILabel("HI " + wp2->getNameTag()) });
							fm->addWidget({ GUIInput(u8"§l§beNPCID（不可重复）") });
							fm->addWidget({ GUIInput(u8"§l§bNPC类型（例:pig,cow）") });
							fm->addWidget({ GUIInput(u8"§l§bNPC标签（显示的名称）") });
							fm->addWidget({ GUIInput(u8"§l§b大小",u8"例：1.5") });
							fm->addWidget({ GUIInput(u8"§l§b触发指令") });
						}
						sendForm(wp2, FullFormBinder{ fm,{[&](WPlayer wp3, FullFormBinder::DType data2) {
							if (!data2.set) return;
							auto& [d1,d2] = data2.val();
							string npcname = std::get<string>(d1[1]);
							string npclx = std::get<string>(d1[2]);
							string npcbq = std::get<string>(d1[3]);
							string npczl = std::get<string>(d1[5]);
							liteloader::runcmdAs(wp3,"npc add " + npcname + " " + npclx + " " + npcbq + " " + npczl);
							}
							} });
					}
					if (data1.val().first == 1) {
						auto npcgui1 = make_shared<SimpleForm>();
						npcgui1->title = u8"§l§e请选者要删除的npc";
						npcgui1->reset();
						for (auto& i : npcs) {
							npcgui1->addButton(GUIButton(string(i.second.name)));
						}
						sendForm(wp2, SimpleFormBinder(npcgui1, [&](ServerPlayer& wp4, SimpleFormBinder::DType data3) {
							if (data3.set) {
								for (auto& i : npcs) {
									if (i.second.name == data3.val().second) {
										auto fms = std::make_shared<FullForm>();

										fms->title = u8"§l§e确认界面";
										fms->addWidget({ GUILabel(u8"§l§eNPCID §a" + i.second.name) });
										fms->addWidget({ GUILabel(u8"§l§e名称 §a" + i.second.nametag) });
										fms->addWidget({ GUILabel(u8"§l§e坐标 §a" + i.second.pos.toString()) });
										fms->addWidget({ GUILabel(u8"§l§e维度 §a" + S(i.second.dim)) });
										fms->addWidget({ GUILabel(u8"§l§e类型 §a" + i.second.type) });
										fms->addWidget({ GUILabel(u8"§l§e大小 §a" + S(i.second.size)) });
										fms->addWidget({ GUILabel(u8"§l§e触发指令 §a" + i.second.data) });
										fms->addWidget({ GUILabel(u8"§l§e旋转 §6俯仰度:§a" + S(i.second.rot.x) + u8" §6旋转度:§a" + S(i.second.rot.y) + u8" §6头部旋转度:§a" + S(i.second.rot.z)) });
										sendForm(wp4, FullFormBinder{ fms,{[&](WPlayer wp5, FullFormBinder::DType dataaa) {
											if (!dataaa.set) return;
												liteloader::runcmdAs(wp5, "npc remove " + i.second.name);
											}
											} });
									}
								}
							}
							}));
					}
					if (data1.val().first == 2) {
						auto npcgui2 = make_shared<SimpleForm>();
						npcgui2->title = u8"§l§eNPC列表";
						npcgui2->reset();
						for (auto& i : npcs) {
							npcgui2->addButton(GUIButton(string(i.second.name)));
						}
						sendForm(wp2, SimpleFormBinder(npcgui2, [&](ServerPlayer& wp7, SimpleFormBinder::DType data4) {
							if (data4.set) {
								for (auto& i : npcs) {
									if (i.second.name == data4.val().second) {
										auto fmsa = std::make_shared<FullForm>();
										fmsa->title = u8"§l§e确认信息";
										fmsa->addWidget({ GUILabel(u8"§l§eNPCID §a" + i.second.name) });
										fmsa->addWidget({ GUILabel(u8"§l§e名称 §a" + i.second.nametag) });
										fmsa->addWidget({ GUILabel(u8"§l§e坐标 §a" + i.second.pos.toString()) });
										fmsa->addWidget({ GUILabel(u8"§l§e维度 §a" + S(i.second.dim)) });
										fmsa->addWidget({ GUILabel(u8"§l§e类型 §a" + i.second.type) });
										fmsa->addWidget({ GUILabel(u8"§l§e大小 §a" + S(i.second.size)) });
										fmsa->addWidget({ GUILabel(u8"§l§e触发指令 §a" + i.second.data) });
										fmsa->addWidget({ GUILabel(u8"§l§e旋转角 §6俯仰角:§a" + S(i.second.rot.x) + u8" §6身体方向度:§a" + S(i.second.rot.y) + u8" §6头部方向度:§a" + S(i.second.rot.z)) });
										sendForm(wp7, FullFormBinder{ fmsa,{[&](ServerPlayer& wp8, FullFormBinder::DType dataaa) {
											if (!dataaa.set) return;
											auto npcguis = make_shared<SimpleForm>();
												npcguis->title = u8"§l§eNPC系统";
												npcguis->content = u8"HI " + wp8->getNameTag();
												npcguis->reset();
												npcguis->addButton(GUIButton(u8"§l§6传送"));
												npcguis->addButton(GUIButton(u8"§l§1修改"));
												npcguis->addButton(GUIButton(u8"§l§c关闭"));
												sendForm(wp8, SimpleFormBinder(npcguis, [&](ServerPlayer& wp9, SimpleFormBinder::DType data5) {
													if (data5.set) {
														if (data5.val().first == 0) {
															wp9.teleport(i.second.pos, i.second.dim);
															wp9.sendText(u8"§l§6[§eNPC§6]§r 已传送至NPC");
														}
														if (data5.val().first == 1) {
															auto npcgui3 = make_shared<SimpleForm>();
															npcgui3->title = u8"§l§e你想修改<" + i.second.name + u8">的什么";
															npcgui3->reset();
															npcgui3->addButton(GUIButton(u8"NPCID"));
															npcgui3->addButton(GUIButton(u8"标签"));
															npcgui3->addButton(GUIButton(u8"坐标"));
															npcgui3->addButton(GUIButton(u8"维度"));
															npcgui3->addButton(GUIButton(u8"类型"));
															npcgui3->addButton(GUIButton(u8"大小"));
															npcgui3->addButton(GUIButton(u8"触发指令"));
															npcgui3->addButton(GUIButton(u8"旋转角"));
															sendForm(wp9, SimpleFormBinder(npcgui3, [&](ServerPlayer& wp10, SimpleFormBinder::DType data6) {
																if (data6.set) {
																	if (data6.val().first == 0) {
																		auto fm10 = std::make_shared<FullForm>();
																		fm10->title = u8"§l§e请修改NPCID";
																		fm10->addWidget({ GUIInput(u8"§l§bNPCID（不可重复）",u8"例：npc123") });
																		sendForm(wp10, FullFormBinder{ fm10,{[&](ServerPlayer& wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			string npcname = std::get<string>(d1[0]);
																			NPC& np = i.second;
																			np.name = npcname;
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.name = npcname;
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPCID修改为" + npcname);
																			}}});
																	}
																	if (data6.val().first == 1) {
																		auto fm10 = std::make_shared<FullForm>();
																		fm10->title = u8"§l§e请修改NPC标签";
																		fm10->addWidget({ GUIInput(u8"§l§bNPC标签（显示的名称）",u8"例：我是商人") });
																		sendForm(wp10, FullFormBinder{ fm10,{[&](ServerPlayer& wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			string out = std::get<string>(d1[0]);
																			NPC& np = i.second;
																			np.nametag = out;
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.nametag = out;
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC标签修改为" + out);
																			}}});
																	}
																	if (data6.val().first == 2) {
																		auto npcgui4 = make_shared<SimpleForm>();
																		npcgui4->title = u8"§l§e你想修改<" + i.second.name + u8">坐标的方式";
																		npcgui4->reset();
																		npcgui4->addButton(GUIButton(u8"传送到你现在的位置"));
																		npcgui4->addButton(GUIButton(u8"指定坐标"));
																		sendForm(wp10, SimpleFormBinder(npcgui4, [&](ServerPlayer& wp11, SimpleFormBinder::DType data7) {
																			if (data7.set) {
																				if (data7.val().first == 0) {
																					NPC& np = i.second;
																					auto a = wp11->getPos();
																					auto out = a.add(0, -1.5, 0);
																					np.pos = out;
																					WBStream ws;
																					ws.apply(np);
																					NPCdb->put("npc_" + i.second.name, ws);
																					i.second.pos = out;
																					broadcastNPCREMOVE(i.second.eid);
																					broadcastNPCADD(i.second.eid);
																					wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的坐标修改为" + out.toString());
																				}
																				if (data7.val().first == 1) {
																					wp11.sendText(u8"§l§6[§eNPC§6]§r 暂时无法使用");
																					/*
																					auto fm11 = std::make_shared<FullForm>();
																					fm11->title = u8"§l§e请修改NPC坐标";
																					fm11->addWidget({ GUIInput(u8"§l§bX") });
																					fm11->addWidget({ GUIInput(u8"§l§bY") });
																					fm11->addWidget({ GUIInput(u8"§l§bZ") });
																					sendForm(wp11, FullFormBinder{ fm11,{[&](WPlayer wp12, FullFormBinder::DType data8) {
																						if (!data8.set) return;
																						auto& [d1, d2] = data8.val();
																						string X = std::get<string>(d1[0]);
																						string Y = std::get<string>(d1[0]);
																						string Z = std::get<string>(d1[0]);
																						Vec3 newpos = { stof(X),stof(Y) ,stof(Z) };
																						NPC& np = i.second;
																						np.pos = newpos;
																						WBStream ws;
																						ws.apply(np);
																						NPCdb->put("npc_" + i.second.name, ws);
																						i.second.pos = newpos;
																						broadcastNPCREMOVE(i.second.eid);
																						broadcastNPCADD(i.second.eid); */
																						//wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC坐标修改为" + newpos.toString());
																					//}}});
																				}
																			}}));
																	}
																	if (data6.val().first == 3) {
																		auto fm13 = std::make_shared<FullForm>();
																		fm13->title = u8"§l§e请修改NPC维度";
																		fm13->addWidget({ GUIDropdown(u8"维度",{"主世界","下界","末地"})});
																		sendForm(wp10, FullFormBinder{ fm13,{[&](ServerPlayer& wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			auto dimid = getNewDimId(d2[0]);
																			if (dimid == -1) {
																				wp11.sendText(u8"§l§6[§eNPC§6]§r 维度错误！");
																				return;
																			}
																			NPC& np = i.second;
																			np.dim = dimid;
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.dim = dimid;
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC维度修改为" + d2[0]);
																		}} });
																	}
																	if (data6.val().first == 4) {
																		auto fm10 = std::make_shared<FullForm>();
																		fm10->title = u8"§l§e请修改NPC类型";
																		fm10->addWidget({ GUIInput(u8"§l§bNPC类型（例:pig,cow）",u8"例：pig") });
																		sendForm(wp10, FullFormBinder{ fm10,{[&](WPlayer wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			string out = std::get<string>(d1[0]);
																			NPC& np = i.second;
																			np.type = out;
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.type = out;
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC类型修改为" + out);
																			}} });
																	}
																	if (data6.val().first == 5) {
																		auto fm10 = std::make_shared<FullForm>();
																		fm10->title = u8"§l§e请修改NPC大小";
																		fm10->addWidget({ GUIInput(u8"§l§b大小",u8"例：1.5") });
																		sendForm(wp10, FullFormBinder{ fm10,{[&](ServerPlayer& wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			string out = std::get<string>(d1[0]);
																			NPC& np = i.second;
																			if (!isNum(out)) {
																				wp11.sendText(u8"§l§6[§eNPC§6]§r 大小出错");
																				return;
																			}
																			np.size = stof(out);
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.size = stof(out);
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC大小修改为" + out);
																			}} });
																	}
																	if (data6.val().first == 6) {
																		auto fm10 = std::make_shared<FullForm>();
																		fm10->title = u8"§l§e请修改NPC触发指令";
																		fm10->addWidget({ GUIInput(u8"§l§b触发指令",u8"例：list") });
																		sendForm(wp10, FullFormBinder{ fm10,{[&](ServerPlayer& wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			string out = std::get<string>(d1[0]);
																			NPC& np = i.second;
																			np.data = out;
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.data = out;
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC类型修改为" + out);
																			}} });
																	}
																	if (data6.val().first == 7) {
																		auto fm10 = std::make_shared<FullForm>();
																		fm10->title = u8"§l§e请修改NPC旋转度";
																		fm10->addWidget({ GUISlider(u8"俯仰角",-180,180,1,i.second.rot.x)});
																		fm10->addWidget({ GUISlider(u8"身体方向角",-180,180,1,i.second.rot.y) });
																		fm10->addWidget({ GUISlider(u8"头部方向角",-180,180,1,i.second.rot.z) });
																		sendForm(wp10, FullFormBinder{ fm10,{[&](ServerPlayer& wp11, FullFormBinder::DType data7) {
																			if (!data7.set) return;
																			auto& [d1, d2] = data7.val();
																			float x = std::get<int>(d1[0]);
																			float y = std::get<int>(d1[1]);
																			float z = std::get<int>(d1[2]);
																			NPC& np = i.second;
																			cout.precision(2);
																			Vec3 a = { x,y,z };
																			np.rot = a;
																			WBStream ws;
																			ws.apply(np);
																			NPCdb->put("npc_" + i.second.name, ws);
																			i.second.rot = a;
																			broadcastNPCREMOVE(i.second.eid);
																			broadcastNPCADD(i.second.eid);
																			wp11.sendText(u8"§l§6[§eNPC§6]§r 已将<" + i.second.name + u8">的NPC类型修改为" + a.toString());
																			}} });
																	}
																	if (data6.val().first == 8) {

																	}
																}
																}));
														}
														if (data5.val().first == 2) {
															return true;
														}
													}
												}));
											}
										} });
									}
								}
							}
							}));
					}
				}
				}));
			break;
		case NPCCommand::NPCType::ADD:
			for (auto& i : npcs) {
				if (i.second.name == name) {
					outp.addMessage(u8"§l§6[§eNPC§6]§r NPC名称重复！");
					return true;
				}
			}
			NPC npc;
			npc.name = name;
			npc.nametag = tag;
			npc.type = MINECRAFT_ENTITY_TYPE(type);
			npc.data = data;
			npc.eid = NPCID++;
			npc.pos = ori.getWorldPosition();
			npc.rot = { 0,0,0 };
			npc.size = 1;
			NPCdb->put("NPCID", to_view(NPCID));
			WBStream ws;
			ws.apply(npc);
			NPCdb->put("npc_" + name, ws);
			int dimid = MakeWP(ori).val().getDimID();
			NPCdb->put("npcdim_" + name, to_view(dimid));
			npc.dim = dimid;
			npcs.emplace(NPCID - 1, std::move(npc));
			broadcastNPCADD(NPCID - 1);
			outp.addMessage("added npc " + name);
			if (iszh(MakeSP(ori))) {
				outp.addMessage(u8"§l§6[§eNPC§6]§r 添加成功\nNPC: " + name + " \nX:" + std::to_string(npc.pos.x) + " \nY:" + std::to_string(npc.pos.y) + " \nZ:" + std::to_string(npc.pos.z));
			}
			else
			{
				outp.addMessage(u8"§l§6[§eNPC§6]§r added success\nNPC: " + name + " \nX:" + std::to_string(npc.pos.x) + " \nY:" + std::to_string(npc.pos.y) + " \nZ:" + std::to_string(npc.pos.z));
			}
			break;
		case NPCCommand::NPCType::SETROT:
				for (auto it = npcs.begin(); it != npcs.end(); ++it) {
					if (it->second.name == name) {
						WPlayer wp = MakeWP(ori).val();
						auto plpos = wp->getPos();
						auto npcpos = it->second.pos;
						//auto x = sqrt((plpos.x - npcpos.x) * (plpos.x - npcpos.x) + (0 - 0) * (0 - 0));
						//auto y = sqrt((0 - 0) * (0 - 0) + (plpos.z - npcpos.z) * (plpos.z - npcpos.z));
						//auto out = atan(y / x);
						Vec2 rots = {};
						auto rot = SymCall("?rotationFromDirection@Vec3@@SA?AVVec2@@AEBV1@@Z", Vec2&, Vec2&, Vec3&)(rots, plpos.add(-npcpos.x, -npcpos.y, -npcpos.z));
						NPC& np = it->second;
						np.rot = { rot.x, rot.z, rot.z };
						WBStream ws;
						ws.apply(np);
						NPCdb->put("npc_" + it->second.name, ws);
						broadcastNPCREMOVE(np.eid);
						broadcastNPCADD(np.eid);
						return true;
					}
				}
				return false;
			break;
		case NPCCommand::NPCType::REMOVE:
			for (auto it = npcs.begin(); it != npcs.end(); ++it) {
				if (it->second.name == name) {
					NPCdb->del("npc_" + name);
					NPCdb->del("npcdata_" + name);
					NPCdb->del("npcdim_" + name);
					auto del = it->first;
					broadcastNPCREMOVE(it->first);
					npcs.erase(del);
					if (iszh(MakeSP(ori))) {
						outp.addMessage(u8"§l§6[§eNPC§6]§r 删除" + name + "!");
					}
					else
					{
						outp.addMessage(u8"§l§6[§eNPC§6]§r deleted" + name + "!");
					}
					return;
				}
			}
			break;
		case NPCCommand::NPCType::DFLAG:
			auto Flag = NPC::getFLAGS(flag);
			for (auto it = npcs.begin(); it != npcs.end(); ++it) {
				if (it->second.name == name) {
					NPC& np = it->second;
					np.flag = Flag;
					broadcastNPCREMOVE(np.eid);
					broadcastNPCADD(np.eid);
					NPCdb->put("npcdata_" + name, flag);
					outp.addMessage(S(flag));
					return;
				}
			}
			break;
		default:
			break;
		}
	}

	static void setup(CommandRegistry* registry) {

		registry->registerCommand("npc","NPC System",CommandPermissionLevel::GameMasters,{ (CommandFlagValue)0 },{ (CommandFlagValue)0x80 });

		registry->addEnum<NPCType>("NPCADD", { {"add", NPCType::ADD} });//ADD
		registry->addEnum<NPCType>("NPCGUI", { {"gui", NPCType::GUI},{"list",NPCType::LIST}});//gui&list
		registry->addEnum<NPCType>("NPCSETROT", { {"setrot", NPCType::SETROT} });//SETROT
		registry->addEnum<NPCType>("NPCREMOVE", { {"remove", NPCType::REMOVE} });//REMOVE
		registry->addEnum<NPCType>("NPCDFLAG", { {"dflag", NPCType::DFLAG} });//DFLAG

		registry->registerOverload<NPCCommand>("npc",
			makeMandatory<CommandParameterDataType::ENUM>(&NPCCommand::operation, "NPCADD","NPCType"),
			makeMandatory(&NPCCommand::addname, "NPCname"),
			makeMandatory(&NPCCommand::addtype, "NPCMobName"),
			makeMandatory(&NPCCommand::addtag, "NPCNameTag"),			
			makeMandatory(&NPCCommand::adddata, "NPCcmd"));

		registry->registerOverload<NPCCommand>("npc",
			makeMandatory<CommandParameterDataType::ENUM>(&NPCCommand::operation, "NPCGUI", "NPCType"),
			makeOptional(&NPCCommand::listoptional, "NPCname",&NPCCommand::listoptional_iset));

		registry->registerOverload<NPCCommand>("npc",
			makeMandatory<CommandParameterDataType::ENUM>(&NPCCommand::operation, "NPCSETROT", "NPCType"),
			makeMandatory(&NPCCommand::setrot, "NPCname"));

		registry->registerOverload<NPCCommand>("npc",
			makeMandatory<CommandParameterDataType::ENUM>(&NPCCommand::operation, "NPCREMOVE", "NPCType"),
			makeMandatory(&NPCCommand::remove, "NPCname"));

		registry->registerOverload<NPCCommand>("npc",
			makeMandatory<CommandParameterDataType::ENUM>(&NPCCommand::operation, "NPCREMOVE", "NPCType"),
			makeMandatory(&NPCCommand::dflagname, "NPCname"),
		    makeMandatory(&NPCCommand::flag, "NPCflag"));
	}
};

void npcc() {

	Schedule::repeat::repeat([] {
		std::vector<Player*> plist = Level::getAllPlayers();
		for (auto p : plist) {
			resendAllNPC((ServerPlayer&)p);
		}
		}, 2);
}



void npcload() {
	NPCdb->iter([](string_view key, string_view val)->bool {
		if (key._Starts_with("npc_")) {
			NPC np;
			RBStream rs(val);
			rs.apply(np);
			auto eid = np.eid;
			npcs.emplace(eid, std::move(np));
		}
		return true;
		});
}



void entry_npc() {
	string val;
	NPCdb = MakeKVDB(GetDataPath("npcutil"));
	if (NPCdb->get("NPCID", val)) {
		memcpy(&NPCID, val.data(), 8);
	}
	npcload();
	npcc();
	Event::addEventListener([](RegCmdEV ev) {

		});
}

THook(int, "?handle@ItemUseOnActorInventoryTransaction@@UEBA?AW4InventoryTransactionError@@AEAVPlayer@@_N@Z", void* thi, ServerPlayer* sp, bool unk) {
	//  public static final int USE_ITEM_ON_ENTITY_ACTION_INTERACT = 0;
	//static final int USE_ITEM_ON_ENTITY_ACTION_ATTACK = 1;
	WBStream ws2;
	auto bpos = sp->getPos();
	auto rtid = dAccess<ActorRuntimeID, 104>(thi);
	auto id = dAccess<int, 112>(thi);
	auto it = npcs.find(rtid);
	if (it != npcs.end()) {
		liteloader::runcmdAs(sp, it->second.data.c_str());
	}
	return original(thi, sp, unk);
}

THook(void, "?respawn@Player@@UEAAXXZ",
	Player* self) {
	original(self);
	resendAllNPC(WPlayer(*self));
}

class ChangeDimensionRequest;
THook(bool, "?_playerChangeDimension@Level@@AEAA_NPEAVPlayer@@AEAVChangeDimensionRequest@@@Z", Level* _this, Player* _this_sp, ChangeDimensionRequest* cdimreq) {
	resendAllNPC(WPlayer(*(ServerPlayer*)_this_sp));
	return  original(_this, _this_sp, cdimreq);
}
