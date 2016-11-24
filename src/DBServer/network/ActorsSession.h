/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/// \addtogroup u2w
/// @{
/// \file

#ifndef __ACTORSESSION_H
#define __ACTORSESSION_H

#include "Common.h"
#include "SharedDefines.h"
#include "ObjectGuid.h"
#include "AuctionHouseMgr.h"
#include "Item.h"
#include "GatewaySocket.h"

#include <deque>
#include <mutex>

struct ItemPrototype;
struct AuctionEntry;
struct AuctionHouseEntry;
struct TradeStatusInfo;

class ObjectGuid;
class Creature;
class Item;
class Object;
class Player;
class Unit;
class GatewayPacket;
class QueryResult;
class LoginQueryHolder;
class CharacterHandler;
class GMTicket;
class MovementInfo;
class GatewaySession;

struct OpcodeHandler;

enum PartyOperation
{
    PARTY_OP_INVITE = 0,
    PARTY_OP_LEAVE = 2,
};

enum PartyResult
{
    ERR_PARTY_RESULT_OK                 = 0,
    ERR_BAD_PLAYER_NAME_S               = 1,
    ERR_TARGET_NOT_IN_GROUP_S           = 2,
    ERR_GROUP_FULL                      = 3,
    ERR_ALREADY_IN_GROUP_S              = 4,
    ERR_NOT_IN_GROUP                    = 5,
    ERR_NOT_LEADER                      = 6,
    ERR_PLAYER_WRONG_FACTION            = 7,
    ERR_IGNORING_YOU_S                  = 8
};

enum TutorialDataState
{
    TUTORIALDATA_UNCHANGED = 0,
    TUTORIALDATA_CHANGED   = 1,
    TUTORIALDATA_NEW       = 2
};

// class to deal with packet processing
// allows to determine if next packet is safe to be processed
class PacketFilter
{
    public:
        explicit PacketFilter(GatewaySession* pSession) : m_pSession(pSession) {}
        virtual ~PacketFilter() {}

        virtual bool Process(GatewayPacket* /*packet*/) { return true; }
        virtual bool ProcessLogout() const { return true; }

    protected:
        GatewaySession* const m_pSession;
};
// process only thread-safe packets in Map::Update()
class MapSessionFilter : public PacketFilter
{
    public:
        explicit MapSessionFilter(GatewaySession* pSession) : PacketFilter(pSession) {}
        ~MapSessionFilter() {}

        virtual bool Process(GatewayPacket* packet) override;
        // in Map::Update() we do not process player logout!
        virtual bool ProcessLogout() const override { return false; }
};

// class used to filer only thread-unsafe packets from queue
// in order to update only be used in Gateway::UpdateSessions()
class GatewaySessionFilter : public PacketFilter
{
    public:
        explicit GatewaySessionFilter(GatewaySession* pSession) : PacketFilter(pSession) {}
        ~GatewaySessionFilter() {}

        virtual bool Process(GatewayPacket* packet) override;
};

/// Player session in the Gateway
class MANGOS_DLL_SPEC GatewaySession
{
        friend class CharacterHandler;

    public:
        GatewaySession(uint32 id, GatewaySocket* sock, AccountTypes sec, time_t mute_time, LocaleConstant locale);
        ~GatewaySession();

        bool PlayerLoading() const { return m_playerLoading; }
        bool PlayerLogout() const { return m_playerLogout; }
        bool PlayerLogoutWithSave() const { return m_playerLogout && m_playerSave; }

        // marks this session as finalized in the socket which owns it.  this lets
        // the socket handling code know that the session can be safely deleted
        void Finalize() { m_Socket->FinalizeSession(); }

        void SizeError(GatewayPacket const& packet, uint32 size) const;

        void SendPacket(GatewayPacket const* packet);
        void SendNotification(const char* format, ...) ATTR_PRINTF(2, 3);
        void SendNotification(int32 string_id, ...);
        void SendPetNameInvalid(uint32 error, const std::string& name);
        void SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res);
        void SendAreaTriggerMessage(const char* Text, ...) ATTR_PRINTF(2, 3);
        void SendTransferAborted(uint32 mapid, uint8 reason, uint8 arg = 0);
        void SendQueryTimeResponse();

        AccountTypes GetSecurity() const { return _security; }
        uint32 GetAccountId() const { return _accountId; }
        Player* GetPlayer() const { return _player; }
        char const* GetPlayerName() const;
        void SetSecurity(AccountTypes security) { _security = security; }
        const std::string &GetRemoteAddress() const { return m_Socket->GetRemoteAddress(); }
        void SetPlayer(Player* plr) { _player = plr; }

        /// Session in auth.queue currently
        void SetInQueue(bool state) { m_inQueue = state; }

        /// Is the user engaged in a log out process?
        bool isLogingOut() const { return _logoutTime || m_playerLogout; }

        /// Engage the logout process for the user
        void LogoutRequest(time_t requestTime)
        {
            _logoutTime = requestTime;
        }

        /// Is logout cooldown expired?
        bool ShouldLogOut(time_t currTime) const
        {
            return (_logoutTime > 0 && currTime >= _logoutTime + 20);
        }

        void LogoutPlayer(bool save);
        void KickPlayer();

        void QueuePacket(GatewayPacket* new_packet);

        bool Update(PacketFilter& updater);

        /// Handle the authentication waiting queue (to be completed)
        void SendAuthWaitQue(uint32 position);

        void SendNameQueryOpcode(Player* p);
        void SendNameQueryOpcodeFromDB(ObjectGuid guid);
        static void SendNameQueryOpcodeFromDBCallBack(QueryResult* result, uint32 accountId);

        void SendTrainerList(ObjectGuid guid);
        void SendTrainerList(ObjectGuid guid, const std::string& strTitle);

        void SendListInventory(ObjectGuid guid);
        bool CheckBanker(ObjectGuid guid);
        void SendShowBank(ObjectGuid guid);
        bool CheckMailBox(ObjectGuid guid);
        void SendTabardVendorActivate(ObjectGuid guid);
        void SendSpiritResurrect();
        void SendBindPoint(Creature* npc);
        void SendGMTicketGetTicket(uint32 status, GMTicket* ticket = nullptr);

        void SendAttackStop(Unit const* enemy);

        void SendBattlegGroundList(ObjectGuid guid, BattleGroundTypeId bgTypeId);

        void SendTradeStatus(const TradeStatusInfo& status);
        void SendUpdateTrade(bool trader_state = true);
        void SendCancelTrade();

        void SendPetitionQueryOpcode(ObjectGuid petitionguid);

        // pet
        void SendPetNameQuery(ObjectGuid guid, uint32 petnumber);
        void SendStablePet(ObjectGuid guid);
        void SendStableResult(uint8 res);
        bool CheckStableMaster(ObjectGuid guid);

        void LoadTutorialsData();
        void SendTutorialsData();
        void SaveTutorialsData();
        uint32 GetTutorialInt(uint32 intId)
        {
            return m_Tutorials[intId];
        }

        void SetTutorialInt(uint32 intId, uint32 value)
        {
            if (m_Tutorials[intId] != value)
            {
                m_Tutorials[intId] = value;
                if (m_tutorialState == TUTORIALDATA_UNCHANGED)
                    m_tutorialState = TUTORIALDATA_CHANGED;
            }
        }

        // auction
        void SendAuctionHello(Unit* unit);
        void SendAuctionCommandResult(AuctionEntry* auc, AuctionAction Action, AuctionError ErrorCode, InventoryResult invError = EQUIP_ERR_OK);
        void SendAuctionBidderNotification(AuctionEntry* auction, bool won);
        void SendAuctionOwnerNotification(AuctionEntry* auction, bool sold);
        void SendAuctionRemovedNotification(AuctionEntry* auction);
        static void SendAuctionOutbiddedMail(AuctionEntry* auction);
        void SendAuctionCancelledToBidderMail(AuctionEntry* auction);
        AuctionHouseEntry const* GetCheckedAuctionHouseForAuctioneer(ObjectGuid guid);

        // Item Enchantment
        void SendEnchantmentLog(ObjectGuid targetGuid, ObjectGuid casterGuid, uint32 itemId, uint32 spellId);
        void SendItemEnchantTimeUpdate(ObjectGuid playerGuid, ObjectGuid itemGuid, uint32 slot, uint32 duration);

        // Taxi
        void SendTaxiStatus(ObjectGuid guid);
        void SendTaxiMenu(Creature* unit);
        void SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode = 0);
        bool SendLearnNewTaxiNode(Creature* unit);
        void SendActivateTaxiReply(ActivateTaxiReply reply);

        // Guild Team
        void SendGuildCommandResult(uint32 typecmd, const std::string& str, uint32 cmdresult);
        void SendPetitionShowList(ObjectGuid guid);
        void SendSaveGuildEmblem(uint32 msg);
        void SendBattleGroundJoinError(uint8 err);

        void BuildPartyMemberStatsChangedPacket(Player* player, GatewayPacket* data);

        // Account mute time
        time_t m_muteTime;

        // Locales
        LocaleConstant GetSessionDbcLocale() const { return m_sessionDbcLocale; }
        int GetSessionDbLocaleIndex() const { return m_sessionDbLocaleIndex; }
        const char* GetMangosString(int32 entry) const;

        uint32 GetLatency() const { return m_latency; }
        void SetLatency(uint32 latency) { m_latency = latency; }
        void ResetClientTimeDelay() { m_clientTimeDelay = 0; }
        uint32 getDialogStatus(Player* pPlayer, Object* questgiver, uint32 defstatus);

        // Misc
        void SendKnockBack(float angle, float horizontalSpeed, float verticalSpeed);
        void SendPlaySpellVisual(ObjectGuid guid, uint32 spellArtKit);

        // opcodes handlers
        void Handle_NULL(GatewayPacket& recvPacket);          // not used
        void Handle_EarlyProccess(GatewayPacket& recvPacket); // just mark packets processed in GatewaySocket::OnRead
        void Handle_ServerSide(GatewayPacket& recvPacket);    // sever side only, can't be accepted from client
        void Handle_Deprecated(GatewayPacket& recvPacket);    // never used anymore by client

        void HandleCharEnumOpcode(GatewayPacket& recvPacket);
        void HandleCharDeleteOpcode(GatewayPacket& recvPacket);
        void HandleCharCreateOpcode(GatewayPacket& recvPacket);
        void HandlePlayerLoginOpcode(GatewayPacket& recvPacket);
        void HandleCharEnum(QueryResult* result);
        void HandlePlayerLogin(LoginQueryHolder* holder);

        // played time
        void HandlePlayedTime(GatewayPacket& recvPacket);

        // new
        void HandleMoveUnRootAck(GatewayPacket& recvPacket);
        void HandleMoveRootAck(GatewayPacket& recvPacket);

        // new inspect
        void HandleInspectOpcode(GatewayPacket& recvPacket);

        // new party stats
        void HandleInspectHonorStatsOpcode(GatewayPacket& recvPacket);

        void HandleMoveWaterWalkAck(GatewayPacket& recvPacket);
        void HandleFeatherFallAck(GatewayPacket& recv_data);

        void HandleMoveHoverAck(GatewayPacket& recv_data);

        void HandleMountSpecialAnimOpcode(GatewayPacket& recvdata);

        // character view
        void HandleShowingHelmOpcode(GatewayPacket& recv_data);
        void HandleShowingCloakOpcode(GatewayPacket& recv_data);

        // repair
        void HandleRepairItemOpcode(GatewayPacket& recvPacket);

        // Knockback
        void HandleMoveKnockBackAck(GatewayPacket& recvPacket);

        void HandleMoveTeleportAckOpcode(GatewayPacket& recvPacket);
        void HandleForceSpeedChangeAckOpcodes(GatewayPacket& recv_data);

        void HandleRepopRequestOpcode(GatewayPacket& recvPacket);
        void HandleAutostoreLootItemOpcode(GatewayPacket& recvPacket);
        void HandleLootMoneyOpcode(GatewayPacket& recvPacket);
        void HandleLootOpcode(GatewayPacket& recvPacket);
        void HandleLootReleaseOpcode(GatewayPacket& recvPacket);
        void HandleLootMasterGiveOpcode(GatewayPacket& recvPacket);
        void HandleWhoOpcode(GatewayPacket& recvPacket);
        void HandleLogoutRequestOpcode(GatewayPacket& recvPacket);
        void HandlePlayerLogoutOpcode(GatewayPacket& recvPacket);
        void HandleLogoutCancelOpcode(GatewayPacket& recvPacket);
        void HandleGMTicketGetTicketOpcode(GatewayPacket& recvPacket);
        void HandleGMTicketCreateOpcode(GatewayPacket& recvPacket);
        void HandleGMTicketSystemStatusOpcode(GatewayPacket& recvPacket);

        void HandleGMTicketDeleteTicketOpcode(GatewayPacket& recvPacket);
        void HandleGMTicketUpdateTextOpcode(GatewayPacket& recvPacket);

        void HandleGMSurveySubmitOpcode(GatewayPacket& recvPacket);

        void HandleTogglePvP(GatewayPacket& recvPacket);

        void HandleZoneUpdateOpcode(GatewayPacket& recvPacket);
        void HandleSetTargetOpcode(GatewayPacket& recvPacket);
        void HandleSetSelectionOpcode(GatewayPacket& recvPacket);
        void HandleStandStateChangeOpcode(GatewayPacket& recvPacket);
        void HandleEmoteOpcode(GatewayPacket& recvPacket);
        void HandleFriendListOpcode(GatewayPacket& recvPacket);
        void HandleAddFriendOpcode(GatewayPacket& recvPacket);
        static void HandleAddFriendOpcodeCallBack(QueryResult* result, uint32 accountId);
        void HandleDelFriendOpcode(GatewayPacket& recvPacket);
        void HandleAddIgnoreOpcode(GatewayPacket& recvPacket);
        static void HandleAddIgnoreOpcodeCallBack(QueryResult* result, uint32 accountId);
        void HandleDelIgnoreOpcode(GatewayPacket& recvPacket);
        void HandleBugOpcode(GatewayPacket& recvPacket);
        void HandleSetAmmoOpcode(GatewayPacket& recvPacket);
        void HandleItemNameQueryOpcode(GatewayPacket& recvPacket);

        void HandleAreaTriggerOpcode(GatewayPacket& recvPacket);

        void HandleSetFactionAtWarOpcode(GatewayPacket& recv_data);
        void HandleSetWatchedFactionOpcode(GatewayPacket& recv_data);
        void HandleSetFactionInactiveOpcode(GatewayPacket& recv_data);

        void HandleUpdateAccountData(GatewayPacket& recvPacket);
        void HandleRequestAccountData(GatewayPacket& recvPacket);
        void HandleSetActionButtonOpcode(GatewayPacket& recvPacket);

        void HandleGameObjectUseOpcode(GatewayPacket& recPacket);
        void HandleMeetingStoneJoinOpcode(GatewayPacket& recPacket);
        void HandleMeetingStoneInfoOpcode(GatewayPacket& recPacket);

        void HandleNameQueryOpcode(GatewayPacket& recvPacket);

        void HandleQueryTimeOpcode(GatewayPacket& recvPacket);

        void HandleCreatureQueryOpcode(GatewayPacket& recvPacket);

        void HandleGameObjectQueryOpcode(GatewayPacket& recvPacket);

        void HandleMoveGatewayportAckOpcode(GatewayPacket& recvPacket);
        void HandleMoveGatewayportAckOpcode();                // for server-side calls

        void HandleMovementOpcodes(GatewayPacket& recvPacket);
        void HandleSetActiveMoverOpcode(GatewayPacket& recv_data);
        void HandleMoveNotActiveMoverOpcode(GatewayPacket& recv_data);
        void HandleMoveTimeSkippedOpcode(GatewayPacket& recv_data);

        void HandleRequestRaidInfoOpcode(GatewayPacket& recv_data);

        void HandleGroupInviteOpcode(GatewayPacket& recvPacket);
        void HandleGroupAcceptOpcode(GatewayPacket& recvPacket);
        void HandleGroupDeclineOpcode(GatewayPacket& recvPacket);
        void HandleGroupUninviteOpcode(GatewayPacket& recvPacket);
        void HandleGroupUninviteGuidOpcode(GatewayPacket& recvPacket);
        void HandleGroupSetLeaderOpcode(GatewayPacket& recvPacket);
        void HandleGroupDisbandOpcode(GatewayPacket& recvPacket);
        void HandleOptOutOfLootOpcode(GatewayPacket& recv_data);
        void HandleLootMethodOpcode(GatewayPacket& recvPacket);
        void HandleLootRoll(GatewayPacket& recv_data);
        void HandleRequestPartyMemberStatsOpcode(GatewayPacket& recv_data);
        void HandleRaidTargetUpdateOpcode(GatewayPacket& recv_data);
        void HandleRaidReadyCheckOpcode(GatewayPacket& recv_data);
        void HandleRaidReadyCheckFinishedOpcode(GatewayPacket& recv_data);
        void HandleGroupRaidConvertOpcode(GatewayPacket& recv_data);
        void HandleGroupChangeSubGroupOpcode(GatewayPacket& recv_data);
        void HandleGroupAssistantLeaderOpcode(GatewayPacket& recv_data);
        void HandlePartyAssignmentOpcode(GatewayPacket& recv_data);

        void HandlePetitionBuyOpcode(GatewayPacket& recv_data);
        void HandlePetitionShowSignOpcode(GatewayPacket& recv_data);
        void HandlePetitionQueryOpcode(GatewayPacket& recv_data);
        void HandlePetitionRenameOpcode(GatewayPacket& recv_data);
        void HandlePetitionSignOpcode(GatewayPacket& recv_data);
        void HandlePetitionDeclineOpcode(GatewayPacket& recv_data);
        void HandleOfferPetitionOpcode(GatewayPacket& recv_data);
        void HandleTurnInPetitionOpcode(GatewayPacket& recv_data);

        void HandleGuildQueryOpcode(GatewayPacket& recvPacket);
        void HandleGuildCreateOpcode(GatewayPacket& recvPacket);
        void HandleGuildInviteOpcode(GatewayPacket& recvPacket);
        void HandleGuildRemoveOpcode(GatewayPacket& recvPacket);
        void HandleGuildAcceptOpcode(GatewayPacket& recvPacket);
        void HandleGuildDeclineOpcode(GatewayPacket& recvPacket);
        void HandleGuildInfoOpcode(GatewayPacket& recvPacket);
        void HandleGuildEventLogQueryOpcode(GatewayPacket& recvPacket);
        void HandleGuildRosterOpcode(GatewayPacket& recvPacket);
        void HandleGuildPromoteOpcode(GatewayPacket& recvPacket);
        void HandleGuildDemoteOpcode(GatewayPacket& recvPacket);
        void HandleGuildLeaveOpcode(GatewayPacket& recvPacket);
        void HandleGuildDisbandOpcode(GatewayPacket& recvPacket);
        void HandleGuildLeaderOpcode(GatewayPacket& recvPacket);
        void HandleGuildMOTDOpcode(GatewayPacket& recvPacket);
        void HandleGuildSetPublicNoteOpcode(GatewayPacket& recvPacket);
        void HandleGuildSetOfficerNoteOpcode(GatewayPacket& recvPacket);
        void HandleGuildRankOpcode(GatewayPacket& recvPacket);
        void HandleGuildAddRankOpcode(GatewayPacket& recvPacket);
        void HandleGuildDelRankOpcode(GatewayPacket& recvPacket);
        void HandleGuildChangeInfoTextOpcode(GatewayPacket& recvPacket);
        void HandleSaveGuildEmblemOpcode(GatewayPacket& recvPacket);

        void HandleTaxiNodeStatusQueryOpcode(GatewayPacket& recvPacket);
        void HandleTaxiQueryAvailableNodes(GatewayPacket& recvPacket);
        void HandleActivateTaxiOpcode(GatewayPacket& recvPacket);
        void HandleActivateTaxiExpressOpcode(GatewayPacket& recvPacket);
        void HandleMoveSplineDoneOpcode(GatewayPacket& recvPacket);

        void HandleTabardVendorActivateOpcode(GatewayPacket& recvPacket);
        void HandleBankerActivateOpcode(GatewayPacket& recvPacket);
        void HandleBuyBankSlotOpcode(GatewayPacket& recvPacket);
        void HandleTrainerListOpcode(GatewayPacket& recvPacket);
        void HandleTrainerBuySpellOpcode(GatewayPacket& recvPacket);

        void HandlePetitionShowListOpcode(GatewayPacket& recvPacket);
        void HandleGossipHelloOpcode(GatewayPacket& recvPacket);
        void HandleGossipSelectOptionOpcode(GatewayPacket& recvPacket);
        void HandleSpiritHealerActivateOpcode(GatewayPacket& recvPacket);
        void HandleNpcTextQueryOpcode(GatewayPacket& recvPacket);
        void HandleBinderActivateOpcode(GatewayPacket& recvPacket);
        void HandleListStabledPetsOpcode(GatewayPacket& recvPacket);
        void HandleStablePet(GatewayPacket& recvPacket);
        void HandleUnstablePet(GatewayPacket& recvPacket);
        void HandleBuyStableSlot(GatewayPacket& recvPacket);
        void HandleStableRevivePet(GatewayPacket& recvPacket);
        void HandleStableSwapPet(GatewayPacket& recvPacket);

        void HandleDuelAcceptedOpcode(GatewayPacket& recvPacket);
        void HandleDuelCancelledOpcode(GatewayPacket& recvPacket);

        void HandleAcceptTradeOpcode(GatewayPacket& recvPacket);
        void HandleBeginTradeOpcode(GatewayPacket& recvPacket);
        void HandleBusyTradeOpcode(GatewayPacket& recvPacket);
        void HandleCancelTradeOpcode(GatewayPacket& recvPacket);
        void HandleClearTradeItemOpcode(GatewayPacket& recvPacket);
        void HandleIgnoreTradeOpcode(GatewayPacket& recvPacket);
        void HandleInitiateTradeOpcode(GatewayPacket& recvPacket);
        void HandleSetTradeGoldOpcode(GatewayPacket& recvPacket);
        void HandleSetTradeItemOpcode(GatewayPacket& recvPacket);
        void HandleUnacceptTradeOpcode(GatewayPacket& recvPacket);

        void HandleAuctionHelloOpcode(GatewayPacket& recvPacket);
        void HandleAuctionListItems(GatewayPacket& recv_data);
        void HandleAuctionListBidderItems(GatewayPacket& recv_data);
        void HandleAuctionSellItem(GatewayPacket& recv_data);

        void HandleAuctionRemoveItem(GatewayPacket& recv_data);
        void HandleAuctionListOwnerItems(GatewayPacket& recv_data);
        void HandleAuctionPlaceBid(GatewayPacket& recv_data);

        void HandleGetMailList(GatewayPacket& recv_data);
        void HandleSendMail(GatewayPacket& recv_data);
        void HandleMailTakeMoney(GatewayPacket& recv_data);
        void HandleMailTakeItem(GatewayPacket& recv_data);
        void HandleMailMarkAsRead(GatewayPacket& recv_data);
        void HandleMailReturnToSender(GatewayPacket& recv_data);
        void HandleMailDelete(GatewayPacket& recv_data);
        void HandleItemTextQuery(GatewayPacket& recv_data);
        void HandleMailCreateTextItem(GatewayPacket& recv_data);
        void HandleQueryNextMailTime(GatewayPacket& recv_data);
        void HandleCancelChanneling(GatewayPacket& recv_data);

        void HandleSplitItemOpcode(GatewayPacket& recvPacket);
        void HandleSwapInvItemOpcode(GatewayPacket& recvPacket);
        void HandleDestroyItemOpcode(GatewayPacket& recvPacket);
        void HandleAutoEquipItemOpcode(GatewayPacket& recvPacket);
        void HandleItemQuerySingleOpcode(GatewayPacket& recvPacket);
        void HandleSellItemOpcode(GatewayPacket& recvPacket);
        void HandleBuyItemInSlotOpcode(GatewayPacket& recvPacket);
        void HandleBuyItemOpcode(GatewayPacket& recvPacket);
        void HandleListInventoryOpcode(GatewayPacket& recvPacket);
        void HandleAutoStoreBagItemOpcode(GatewayPacket& recvPacket);
        void HandleReadItemOpcode(GatewayPacket& recvPacket);
        void HandleAutoEquipItemSlotOpcode(GatewayPacket& recvPacket);
        void HandleSwapItem(GatewayPacket& recvPacket);
        void HandleBuybackItem(GatewayPacket& recvPacket);
        void HandleAutoBankItemOpcode(GatewayPacket& recvPacket);
        void HandleAutoStoreBankItemOpcode(GatewayPacket& recvPacket);
        void HandleWrapItemOpcode(GatewayPacket& recvPacket);

        void HandleAttackSwingOpcode(GatewayPacket& recvPacket);
        void HandleAttackStopOpcode(GatewayPacket& recvPacket);
        void HandleSetSheathedOpcode(GatewayPacket& recvPacket);

        void HandleUseItemOpcode(GatewayPacket& recvPacket);
        void HandleOpenItemOpcode(GatewayPacket& recvPacket);
        void HandleCastSpellOpcode(GatewayPacket& recvPacket);
        void HandleCancelCastOpcode(GatewayPacket& recvPacket);
        void HandleCancelAuraOpcode(GatewayPacket& recvPacket);
        void HandleCancelGrowthAuraOpcode(GatewayPacket& recvPacket);
        void HandleCancelAutoRepeatSpellOpcode(GatewayPacket& recvPacket);

        void HandleLearnTalentOpcode(GatewayPacket& recvPacket);
        void HandleTalentWipeConfirmOpcode(GatewayPacket& recvPacket);
        void HandleUnlearnSkillOpcode(GatewayPacket& recvPacket);

        void HandleQuestgiverStatusQueryOpcode(GatewayPacket& recvPacket);
        void HandleQuestgiverStatusMultipleQuery(GatewayPacket& recvPacket);
        void HandleQuestgiverHelloOpcode(GatewayPacket& recvPacket);
        void HandleQuestgiverAcceptQuestOpcode(GatewayPacket& recvPacket);
        void HandleQuestgiverQueryQuestOpcode(GatewayPacket& recvPacket);
        void HandleQuestgiverChooseRewardOpcode(GatewayPacket& recvPacket);
        void HandleQuestgiverRequestRewardOpcode(GatewayPacket& recvPacket);
        void HandleQuestQueryOpcode(GatewayPacket& recvPacket);
        void HandleQuestgiverCancel(GatewayPacket& recv_data);
        void HandleQuestLogSwapQuest(GatewayPacket& recv_data);
        void HandleQuestLogRemoveQuest(GatewayPacket& recv_data);
        void HandleQuestConfirmAccept(GatewayPacket& recv_data);
        void HandleQuestgiverCompleteQuest(GatewayPacket& recv_data);
        bool CanInteractWithQuestGiver(ObjectGuid guid, char const* descr);

        void HandleQuestgiverQuestAutoLaunch(GatewayPacket& recvPacket);
        void HandlePushQuestToParty(GatewayPacket& recvPacket);
        void HandleQuestPushResult(GatewayPacket& recvPacket);

        bool processChatmessageFurtherAfterSecurityChecks(std::string&, uint32);
        void SendPlayerNotFoundNotice(const std::string& name);
        void SendWrongFactionNotice();
        void SendChatRestrictedNotice();
        void HandleMessagechatOpcode(GatewayPacket& recvPacket);
        void HandleTextEmoteOpcode(GatewayPacket& recvPacket);
        void HandleChatIgnoredOpcode(GatewayPacket& recvPacket);

        void HandleReclaimCorpseOpcode(GatewayPacket& recvPacket);
        void HandleCorpseQueryOpcode(GatewayPacket& recvPacket);
        void HandleResurrectResponseOpcode(GatewayPacket& recvPacket);
        void HandleSummonResponseOpcode(GatewayPacket& recv_data);

        void HandleJoinChannelOpcode(GatewayPacket& recvPacket);
        void HandleLeaveChannelOpcode(GatewayPacket& recvPacket);
        void HandleChannelListOpcode(GatewayPacket& recvPacket);
        void HandleChannelPasswordOpcode(GatewayPacket& recvPacket);
        void HandleChannelSetOwnerOpcode(GatewayPacket& recvPacket);
        void HandleChannelOwnerOpcode(GatewayPacket& recvPacket);
        void HandleChannelModeratorOpcode(GatewayPacket& recvPacket);
        void HandleChannelUnmoderatorOpcode(GatewayPacket& recvPacket);
        void HandleChannelMuteOpcode(GatewayPacket& recvPacket);
        void HandleChannelUnmuteOpcode(GatewayPacket& recvPacket);
        void HandleChannelInviteOpcode(GatewayPacket& recvPacket);
        void HandleChannelKickOpcode(GatewayPacket& recvPacket);
        void HandleChannelBanOpcode(GatewayPacket& recvPacket);
        void HandleChannelUnbanOpcode(GatewayPacket& recvPacket);
        void HandleChannelAnnouncementsOpcode(GatewayPacket& recvPacket);
        void HandleChannelModerateOpcode(GatewayPacket& recvPacket);
        void HandleChannelDisplayListQueryOpcode(GatewayPacket& recvPacket);
        void HandleGetChannelMemberCountOpcode(GatewayPacket& recvPacket);
        void HandleSetChannelWatchOpcode(GatewayPacket& recvPacket);

        void HandleCompleteCinematic(GatewayPacket& recvPacket);
        void HandleNextCinematicCamera(GatewayPacket& recvPacket);

        void HandlePageQuerySkippedOpcode(GatewayPacket& recvPacket);
        void HandlePageTextQueryOpcode(GatewayPacket& recvPacket);

        void HandleTutorialFlagOpcode(GatewayPacket& recv_data);
        void HandleTutorialClearOpcode(GatewayPacket& recv_data);
        void HandleTutorialResetOpcode(GatewayPacket& recv_data);

        // Pet
        void HandlePetAction(GatewayPacket& recv_data);
        void HandlePetStopAttack(GatewayPacket& recv_data);
        void HandlePetNameQueryOpcode(GatewayPacket& recv_data);
        void HandlePetSetAction(GatewayPacket& recv_data);
        void HandlePetAbandon(GatewayPacket& recv_data);
        void HandlePetRename(GatewayPacket& recv_data);
        void HandlePetCancelAuraOpcode(GatewayPacket& recvPacket);
        void HandlePetUnlearnOpcode(GatewayPacket& recvPacket);
        void HandlePetSpellAutocastOpcode(GatewayPacket& recvPacket);
        void HandlePetCastSpellOpcode(GatewayPacket& recvPacket);

        void HandleSetActionBarTogglesOpcode(GatewayPacket& recv_data);

        void HandleCharRenameOpcode(GatewayPacket& recv_data);
        static void HandleChangePlayerNameOpcodeCallBack(QueryResult* result, uint32 accountId, std::string newname);

        void HandleTotemDestroyed(GatewayPacket& recv_data);

        // BattleGround
        void HandleBattlemasterHelloOpcode(GatewayPacket& recv_data);
        void HandleBattlemasterJoinOpcode(GatewayPacket& recv_data);
        void HandleBattleGroundPlayerPositionsOpcode(GatewayPacket& recv_data);
        void HandlePVPLogDataOpcode(GatewayPacket& recv_data);
        void HandleBattlefieldStatusOpcode(GatewayPacket& recv_data);
        void HandleBattleFieldPortOpcode(GatewayPacket& recv_data);
        void HandleBattlefieldListOpcode(GatewayPacket& recv_data);
        void HandleLeaveBattlefieldOpcode(GatewayPacket& recv_data);

        void HandleWardenDataOpcode(GatewayPacket& recv_data);
        void HandleGatewayTeleportOpcode(GatewayPacket& recv_data);
        void HandleMinimapPingOpcode(GatewayPacket& recv_data);
        void HandleRandomRollOpcode(GatewayPacket& recv_data);
        void HandleFarSightOpcode(GatewayPacket& recv_data);
        void HandleWhoisOpcode(GatewayPacket& recv_data);
        void HandleResetInstancesOpcode(GatewayPacket& recv_data);

        void HandleAreaSpiritHealerQueryOpcode(GatewayPacket& recv_data);
        void HandleAreaSpiritHealerQueueOpcode(GatewayPacket& recv_data);
        void HandleCancelMountAuraOpcode(GatewayPacket& recv_data);
        void HandleSelfResOpcode(GatewayPacket& recv_data);
        void HandleRequestPetInfoOpcode(GatewayPacket& recv_data);

        void HandleCancelTempEnchantmentOpcode(GatewayPacket& recv_data);

        void HandleSetTaxiBenchmarkOpcode(GatewayPacket& recv_data);

    private:
        // private trade methods
        void moveItems(Item* myItems[], Item* hisItems[]);
        bool VerifyMovementInfo(MovementInfo const& movementInfo, ObjectGuid const& guid) const;
        bool VerifyMovementInfo(MovementInfo const& movementInfo) const;
        void HandleMoverRelocation(MovementInfo& movementInfo);

        void ExecuteOpcode(OpcodeHandler const& opHandle, GatewayPacket* packet);

        // logging helper
        void LogUnexpectedOpcode(GatewayPacket* packet, const char* reason);
        void LogUnprocessedTail(GatewayPacket* packet);

        std::mutex m_logoutMutex;                           // this mutex is necessary to avoid two simultaneous logouts due to a valid logout request and socket error
        Player * _player;
        GatewaySocket * const m_Socket;                       // socket pointer is owned by the network thread which created 

        AccountTypes _security;
        uint32 _accountId;

        time_t _logoutTime;
        bool m_inQueue;                                     // session wait in auth.queue
        bool m_playerLoading;                               // code processed in LoginPlayer

        // True when the player is in the process of logging out (GatewaySession::LogoutPlayer is currently executing)
        bool m_playerLogout;
        bool m_playerRecentlyLogout;
        bool m_playerSave;                                  // code processed in LogoutPlayer with save request
        LocaleConstant m_sessionDbcLocale;
        int m_sessionDbLocaleIndex;
        uint32 m_latency;
        uint32 m_clientTimeDelay;
        uint32 m_Tutorials[8];
        TutorialDataState m_tutorialState;

        std::mutex m_recvQueueLock;
        std::deque<GatewayPacket *> m_recvQueue;
};
#endif
/// @}
