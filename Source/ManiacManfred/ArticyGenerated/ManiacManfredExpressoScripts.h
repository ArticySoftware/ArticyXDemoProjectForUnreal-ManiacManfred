﻿// articy Software GmbH & Co. KG
/** This code file was generated by ArticyImporter. Changes to this file will get lost once the code is regenerated. */

#pragma once

#include "CoreUObject.h"
#include "ArticyExpressoScripts.h"
#include "ManiacManfredGlobalVariables.h"
#include "ManiacManfredExpressoScripts.generated.h"

UINTERFACE(Blueprintable)
class UManiacManfredMethodsProvider : public UInterface { GENERATED_BODY() };
class IManiacManfredMethodsProvider
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Articy Methods Provider", meta=(DisplayName="restart"))
	void restart() ;
	virtual void restart_Implementation() {  };
};

UCLASS(BlueprintType, Blueprintable)
class MANIACMANFRED_API UManiacManfredExpressoScripts : public UArticyExpressoScripts
{
	GENERATED_BODY()
	
private:
	
	void restart() const
	{
		auto methodProvider = GetUserMethodsProviderObject();
		if(!methodProvider) return ;
		IManiacManfredMethodsProvider::Execute_restart(methodProvider);
	}
	
private:
	
	mutable TWeakObjectPtr<UManiacManfredGameStateVariables> GameState = nullptr;
	mutable TWeakObjectPtr<UManiacManfredInventoryVariables> Inventory = nullptr;
	mutable TWeakObjectPtr<UManiacManfredGlobalVariables> ActiveGlobals = nullptr;
	
	void SetGV(UArticyGlobalVariables* GV) const override
	{
		auto gv = Cast<UManiacManfredGlobalVariables>(GV);
		/** Initialize all GV namespace references (or nullify if we're setting to nullptr) */
		GameState = gv ? gv->GameState : nullptr;
		Inventory = gv ? gv->Inventory : nullptr;
		/** Store GVs */
		ActiveGlobals = gv;
	}
	
	UArticyGlobalVariables* GetGV() override
	{
		/** Return active global variables as set by SetGV */
		if(ActiveGlobals.IsValid()) { return ActiveGlobals.Get(); }
		return nullptr;
	}
	
	UClass* GetUserMethodsProviderInterface() override
	{
		return UManiacManfredMethodsProvider::StaticClass();
	}
	
public:
	
	#if !((defined(PLATFORM_PS4) && PLATFORM_PS4) || (defined(PLATFORM_PS5) && PLATFORM_PS5))
	#pragma warning(push)
	#pragma warning(disable: 4883) //<disable "optimization cannot be applied due to function size" compile error.
	#endif
	 UManiacManfredExpressoScripts() 
	{
		Conditions.Add(157729511, [&]
		{
			return ConditionOrTrue(
				//Clicking on therapist

			);
		});
		Instructions.Add(952325012, [&]
		{
			//Knocked down therapist

		});
		Instructions.Add(391467238, [&]
		{
			//Key stolen

		});
		Instructions.Add(1296299463, [&]
		{
			//Convinced therapist

		});
		Instructions.Add(-726194376, [&]
		{
			//Leaving dialogue without result

		});
		Conditions.Add(1531033253, [&]
		{
			return ConditionOrTrue(
				//Clicking on door without key

			);
		});
		Conditions.Add(-1487264577, [&]
		{
			return ConditionOrTrue(
				//Clicking on door with key

			);
		});
		Instructions.Add(1853193494, [&]
		{
			//Door still closed

		});
		Instructions.Add(1336799699, [&]
		{
			//Door open

		});
		Conditions.Add(-622149624, [&]
		{
			return ConditionOrTrue(
				//Clicking on umbrella

			);
		});
		Conditions.Add(-387889608, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == false
			);
		});
		Instructions.Add(-1320870045, [&]
		{
			//Knock out with crowbar

		});
		Instructions.Add(621897570, [&]
		{
			//Three-headed giraffe

		});
		Conditions.Add(-960518206, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == true
			);
		});
		Instructions.Add(-38208709, [&]
		{
			//Banana with sleeping pills

		});
		Instructions.Add(-2088938373, [&]
		{
			//Crowbar

		});
		Conditions.Add(1157659929, [&]
		{
			return ConditionOrTrue(
				(*GameState->awake) == false
			);
		});
		Instructions.Add(514714048, [&]
		{
			(*GameState->awake) = true;
		});
		Conditions.Add(1087258897, [&]
		{
			return ConditionOrTrue(
				(*GameState->looney_bin) == false
			);
		});
		Instructions.Add(1369332579, [&]
		{
			(*GameState->looney_bin) = true;
		});
		Conditions.Add(1009897472, [&]
		{
			return ConditionOrTrue(
				(*GameState->memory) == false
			);
		});
		Instructions.Add(1473601671, [&]
		{
			(*GameState->memory) = true;
		});
		Conditions.Add(1420154279, [&]
		{
			return ConditionOrTrue(
				(*GameState->memory) == true && (*GameState->looney_bin) == true
			);
		});
		Instructions.Add(-21163797, [&]
		{
			(*Inventory->key) = true;
		});
		Conditions.Add(1681566196, [&]
		{
			return ConditionOrTrue(
				(*GameState->awake) == true
			);
		});
		Conditions.Add(475933664, [&]
		{
			return ConditionOrTrue(
				//Inspecting knocked-out therapist

			);
		});
		Instructions.Add(-1134785724, [&]
		{
			(*GameState->therapist_gone) = true;
 (*GameState->door_open) = true;
 (*GameState->therapist_convinced) = true;
		});
		Instructions.Add(-1043421293, [&]
		{
			(*GameState->therapist_gone) = true;
		});
		Conditions.Add(-1204673011, [&]
		{
			return ConditionOrTrue(
				(*GameState->looted) == false
			);
		});
		Instructions.Add(-507541619, [&]
		{
			(*GameState->looted) = true;
		});
		Conditions.Add(373250911, [&]
		{
			return ConditionOrTrue(
				(*GameState->looted) == true
			);
		});
		Conditions.Add(1649070299, [&]
		{
			return ConditionOrTrue(
				//knocking out the therapist

			);
		});
		Conditions.Add(591853018, [&]
		{
			return ConditionOrTrue(
				//trying to beat the knocked-out therapist

			);
		});
		Instructions.Add(1440624735, [&]
		{
			(*Inventory->crowbar) = true;
		});
		Conditions.Add(2044064390, [&]
		{
			return ConditionOrTrue(
				(*GameState->listenedToVoice) == false
			);
		});
		Instructions.Add(-658597971, [&]
		{
			(*GameState->listenedToVoice) = true;
		});
		Instructions.Add(523884011, [&]
		{
			(*GameState->dialogue_beforeLobby) = true;
		});
		Instructions.Add(363156250, [&]
		{
			(*GameState->dialogue_beforeCellar) = true;
		});
		Conditions.Add(779452482, [&]
		{
			return ConditionOrTrue(
				(*GameState->hamster_talkedTo) == false
			);
		});
		Instructions.Add(-1287269406, [&]
		{
			(*Inventory->opener) = true;
		});
		Conditions.Add(-789049156, [&]
		{
			return ConditionOrTrue(
				(*Inventory->opener) == false
			);
		});
		Instructions.Add(-68006091, [&]
		{
			(*Inventory->opener) = true;
		});
		Instructions.Add(560346354, [&]
		{
			(*GameState->hamster_talkedTo) = true;
		});
		Instructions.Add(2147089890, [&]
		{
			(*Inventory->hamster) = true;
 (*GameState->hamster_saved) = true;
		});
		Conditions.Add(-1251659804, [&]
		{
			return ConditionOrTrue(
				(*GameState->hamster_talkedTo) == true
			);
		});
		Conditions.Add(1273544474, [&]
		{
			return ConditionOrTrue(
				(*Inventory->opener) == true
			);
		});
		Conditions.Add(596692500, [&]
		{
			return ConditionOrTrue(
				(*Inventory->aluminium) == false && (*Inventory->bomb) == false
			);
		});
		Instructions.Add(1779669151, [&]
		{
			(*Inventory->aluminium) = true;
		});
		Conditions.Add(-1363297488, [&]
		{
			return ConditionOrTrue(
				(*Inventory->aluminium) == true || (*Inventory->bomb) == true
			);
		});
		Instructions.Add(-496580677, [&]
		{
			(*Inventory->plutonium) = true;
		});
		Conditions.Add(-1390611271, [&]
		{
			return ConditionOrTrue(
				(*GameState->overflow_open) == true && ((*Inventory->plutonium) == true || (*Inventory->detonator) == true || (*Inventory->enrichedPlutonium) == true || (*Inventory->bomb) == true)
			);
		});
		Conditions.Add(-569545171, [&]
		{
			return ConditionOrTrue(
				(*GameState->overflow_open) == false
			);
		});
		Conditions.Add(-12465753, [&]
		{
			return ConditionOrTrue(
				(*GameState->overflow_open) == true && !((*Inventory->plutonium) == true || (*Inventory->enrichedPlutonium) == true || (*Inventory->bomb) == true)
			);
		});
		Instructions.Add(1961281764, [&]
		{
			(*Inventory->cable) = true;
		});
		Instructions.Add(-1774062208, [&]
		{
			(*Inventory->broom) = true;
		});
		Conditions.Add(1945233822, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == true
			);
		});
		Conditions.Add(-897792467, [&]
		{
			return ConditionOrTrue(
				(*GameState->book_read) == false
			);
		});
		Instructions.Add(753324628, [&]
		{
			(*GameState->book_read) = true;
		});
		Conditions.Add(-1014333346, [&]
		{
			return ConditionOrTrue(
				(*GameState->locker_open) == true &&  (*Inventory->constructionKit) == false &&  (*Inventory->enrichedPlutonium) == false &&  (*Inventory->detonator) == false && (*Inventory->bomb) == false
			);
		});
		Instructions.Add(980552993, [&]
		{
			(*Inventory->constructionKit) = true;
		});
		Conditions.Add(1580432211, [&]
		{
			return ConditionOrTrue(
				(*GameState->locker_open) == false
			);
		});
		Instructions.Add(896137014, [&]
		{
			(*GameState->locker_open) = true;
		});
		Instructions.Add(-877765446, [&]
		{
			(*GameState->lock_number) = 0;
 (*GameState->lock_correctNumbers) = 0;
		});
		Conditions.Add(-658783541, [&]
		{
			return ConditionOrTrue(
				getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) >= 10
			);
		});
		Conditions.Add(-119970820, [&]
		{
			return ConditionOrTrue(
				getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue")))  < 10 && getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) > -10
			);
		});
		Conditions.Add(-1122005591, [&]
		{
			return ConditionOrTrue(
				getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) <= -10
			);
		});
		Conditions.Add(-554843393, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_convinced) == true && (*GameState->exit_open) == false && (*Inventory->bananaPill) == false
			);
		});
		Conditions.Add(505227975, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_convinced) == false && (*GameState->exit_open) == false && (*Inventory->bananaPill) == false
			);
		});
		Conditions.Add(1131182839, [&]
		{
			return ConditionOrTrue(
				(*Inventory->crowbar) == true
			);
		});
		Instructions.Add(1854889839, [&]
		{
			(*GameState->therapist_knockedOut2) = true;
		});
		Instructions.Add(1441548157, [&]
		{
			(*GameState->exit_open) = true;
		});
		Conditions.Add(-548535918, [&]
		{
			return ConditionOrTrue(
				(*GameState->guard_met) == false && (*GameState->therapist_knockedOut) == true
			);
		});
		Instructions.Add(-1076726596, [&]
		{
			(*GameState->guard_met) = true;
		});
		Instructions.Add(-433269607, [&]
		{
			(*GameState->exit_open) = true;
 (*GameState->guard_drugged) = true;
		});
		Instructions.Add(-949978205, [&]
		{
			(*GameState->exit_open) = true;
 (*GameState->guard_knockedOut) = true;
		});
		Conditions.Add(1386286727, [&]
		{
			return ConditionOrTrue(
				(*GameState->guard_met) == true && (*Inventory->bananaPill) == false && (*GameState->therapist_knockedOut) == true && (*GameState->guard_knockedOut) == false
			);
		});
		Conditions.Add(1135064469, [&]
		{
			return ConditionOrTrue(
				(*GameState->guard_knockedOut) == true
			);
		});
		Instructions.Add(834208332, [&]
		{
			(*Inventory->sleepingPills) = true;
		});
		Conditions.Add(-274178413, [&]
		{
			return ConditionOrTrue(
				//combine key with something useless

			);
		});
		Conditions.Add(1125298467, [&]
		{
			return ConditionOrTrue(
				//combine crowbar with something useless

			);
		});
		Conditions.Add(1799622746, [&]
		{
			return ConditionOrTrue(
				//combine cable with something useless

			);
		});
		Conditions.Add(-27516474, [&]
		{
			return ConditionOrTrue(
				//combine broom with something useless

			);
		});
		Conditions.Add(1552648553, [&]
		{
			return ConditionOrTrue(
				(*Inventory->crowbar) == true
			);
		});
		Instructions.Add(-806740236, [&]
		{
			(*GameState->therapist_knockedOut) = true;
		});
		Instructions.Add(-2078302858, [&]
		{
			setProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue")), getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) - 15);
		});
		Instructions.Add(1347204775, [&]
		{
			setProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue")), getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) + 10);
		});
		Conditions.Add(-41092919, [&]
		{
			return ConditionOrTrue(
				//use key with door

			);
		});
		Instructions.Add(75263553, [&]
		{
			setProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue")), getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) - 10);
		});
		Instructions.Add(2102035221, [&]
		{
			setProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue")), getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) - 5);
		});
		Instructions.Add(-4087174, [&]
		{
			// using the bomb to get out of the sanitarium.

		});
		Conditions.Add(-493619538, [&]
		{
			return ConditionOrTrue(
				(*GameState->book_read) == true
			);
		});
		Instructions.Add(-1773249919, [&]
		{
			(*GameState->exit_open) = true;
 (*GameState->therapist_down) = true;
 (*GameState->therapist_knockedOut2) = true;
		});
		Instructions.Add(-541920990, [&]
		{
			(*GameState->exit_open) = true;
		});
		Conditions.Add(1411941048, [&]
		{
			return ConditionOrTrue(
				(*GameState->exit_open) == false && (*Inventory->bananaPill) == true
			);
		});
		Conditions.Add(-1915194140, [&]
		{
			return ConditionOrTrue(
				(*GameState->exit_open) == true && (*GameState->therapist_down) == true
			);
		});
		Conditions.Add(-965193106, [&]
		{
			return ConditionOrTrue(
				(*GameState->guard_met) == true && (*Inventory->bananaPill) && (*GameState->therapist_knockedOut) == true && (*GameState->exit_open) == false && (*GameState->guard_knockedOut) == false
			);
		});
		Conditions.Add(993429672, [&]
		{
			return ConditionOrTrue(
				(*GameState->hamster_saved) == true
			);
		});
		Conditions.Add(-1081056416, [&]
		{
			return ConditionOrTrue(
				(*GameState->lock_number) == 3
			);
		});
		Instructions.Add(-670818344, [&]
		{
			(*GameState->lock_correctNumbers) += 1;
		});
		Conditions.Add(800229318, [&]
		{
			return ConditionOrTrue(
				(*GameState->lock_number) == 1
			);
		});
		Conditions.Add(-1832302238, [&]
		{
			return ConditionOrTrue(
				(*GameState->lock_number) < 4
			);
		});
		Instructions.Add(729650942, [&]
		{
			(*GameState->lock_number) += 1;
		});
		Conditions.Add(-80824049, [&]
		{
			return ConditionOrTrue(
				(*GameState->lock_correctNumbers) >= 4
			);
		});
		Conditions.Add(1096916417, [&]
		{
			return ConditionOrTrue(
				(*GameState->lock_number) == 2 || (*GameState->lock_number) == 4
			);
		});
		Conditions.Add(412840955, [&]
		{
			return ConditionOrTrue(
				(*Inventory->constructionKit)
			);
		});
		Conditions.Add(-173324159, [&]
		{
			return ConditionOrTrue(
				(*Inventory->sleepingPills)
			);
		});
		Conditions.Add(1783967320, [&]
		{
			return ConditionOrTrue(
				(*Inventory->plutonium)
			);
		});
		Conditions.Add(1406753786, [&]
		{
			return ConditionOrTrue(
				(*Inventory->key)
			);
		});
		Conditions.Add(679718394, [&]
		{
			return ConditionOrTrue(
				(*Inventory->crowbar)
			);
		});
		Conditions.Add(-972086420, [&]
		{
			return ConditionOrTrue(
				(*Inventory->cable)
			);
		});
		Conditions.Add(-1161654298, [&]
		{
			return ConditionOrTrue(
				(*Inventory->bomb)
			);
		});
		Conditions.Add(227797041, [&]
		{
			return ConditionOrTrue(
				(*Inventory->banana)
			);
		});
		Conditions.Add(171906384, [&]
		{
			return ConditionOrTrue(
				(*Inventory->aluminium)
			);
		});
		Conditions.Add(1730728460, [&]
		{
			return ConditionOrTrue(
				(*Inventory->enrichedPlutonium)
			);
		});
		Conditions.Add(-1102301490, [&]
		{
			return ConditionOrTrue(
				(*Inventory->detonator)
			);
		});
		Conditions.Add(1423372899, [&]
		{
			return ConditionOrTrue(
				(*Inventory->hamster)
			);
		});
		Conditions.Add(1812841104, [&]
		{
			return ConditionOrTrue(
				(*Inventory->bananaPill)
			);
		});
		Conditions.Add(-208586440, [&]
		{
			return ConditionOrTrue(
				(*Inventory->broom)
			);
		});
		Conditions.Add(-212134596, [&]
		{
			return ConditionOrTrue(
				(*Inventory->opener)
			);
		});
		Conditions.Add(-394605648, [&]
		{
			return ConditionOrTrue(
				(*GameState->dialogue_beforeLobby) == true
			);
		});
		Conditions.Add(-1706754119, [&]
		{
			return ConditionOrTrue(
				(*GameState->dialogue_beforeCellar) == true
			);
		});
		Conditions.Add(-342217906, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == true && (*GameState->guard_knockedOut) == false && (*GameState->guard_drugged) == false
			);
		});
		Conditions.Add(927641006, [&]
		{
			return ConditionOrTrue(
				(*GameState->guard_knockedOut) == false && (*GameState->therapist_knockedOut) == true
			);
		});
		Conditions.Add(1641608681, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == false &&  (*GameState->therapist_knockedOut2) == false
			);
		});
		Conditions.Add(1368712095, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_down) == false && (*GameState->therapist_knockedOut) == false
			);
		});
		Conditions.Add(-37153472, [&]
		{
			return ConditionOrTrue(
				(*Inventory->sleepingPills) ==  false && (*Inventory->bananaPill) == false
			);
		});
		Conditions.Add(-1527032372, [&]
		{
			return ConditionOrTrue(
				(*Inventory->banana) == false && (*Inventory->bananaPill) == false
			);
		});
		Conditions.Add(1555693131, [&]
		{
			return ConditionOrTrue(
				(*GameState->exit_open) == true
			);
		});
		Conditions.Add(1614311782, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut2) == true
			);
		});
		Instructions.Add(-1339129793, [&]
		{
			restart();
		});
		Conditions.Add(-108403695, [&]
		{
			return ConditionOrTrue(
				(*Inventory->broom) == false
			);
		});
		Conditions.Add(-344587501, [&]
		{
			return ConditionOrTrue(
				(*Inventory->cable) == false && (*Inventory->detonator) == false && (*Inventory->bomb) == false
			);
		});
		Instructions.Add(-1907382023, [&]
		{
			(*GameState->overflow_open) = true;
		});
		Conditions.Add(2018112060, [&]
		{
			return ConditionOrTrue(
				(*GameState->hamster_saved) == false
			);
		});
		Conditions.Add(-1366845359, [&]
		{
			return ConditionOrTrue(
				(*GameState->hamster_talkedTo) == true
			);
		});
		Conditions.Add(-1742024854, [&]
		{
			return ConditionOrTrue(
				(*GameState->locker_open) == false
			);
		});
		Conditions.Add(1417430945, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == false && (*GameState->therapist_gone) == false
			);
		});
		Instructions.Add(-1383730238, [&]
		{
			(*GameState->therapist_knockedOut) = true;
 setProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue")), getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) - 10);
		});
		Conditions.Add(-662907567, [&]
		{
			return ConditionOrTrue(
				(*Inventory->crowbar) == false
			);
		});
		Conditions.Add(484196418, [&]
		{
			return ConditionOrTrue(
				(*GameState->door_open) == true
			);
		});
		Conditions.Add(-901685068, [&]
		{
			return ConditionOrTrue(
				(*GameState->door_open) == false
			);
		});
		Instructions.Add(1233965202, [&]
		{
			(*GameState->door_open) = true;
		});
		Conditions.Add(1628394034, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut2) == false && (*GameState->therapist_knockedOut) == false && (*GameState->therapist_gone) == true
			);
		});
		Conditions.Add(-697625804, [&]
		{
			return ConditionOrTrue(
				(*Inventory->sleepingPills) == false
			);
		});
		Conditions.Add(-449642799, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == true && (*GameState->guard_drugged) == false && (*GameState->guard_knockedOut) == false
			);
		});
		Conditions.Add(1944664492, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == true && (*GameState->guard_drugged) == false && (*GameState->guard_knockedOut) == true
			);
		});
		Conditions.Add(1503100157, [&]
		{
			return ConditionOrTrue(
				(*GameState->therapist_knockedOut) == true && (*GameState->guard_drugged) == true && (*GameState->guard_knockedOut) == false
			);
		});
		Conditions.Add(1765091100, [&]
		{
			return ConditionOrTrue(
				(*Inventory->enrichedPlutonium) == false
			);
		});
		Conditions.Add(1010585088, [&]
		{
			return ConditionOrTrue(
				(*GameState->locker_open) == true
			);
		});
		Conditions.Add(-1157422898, [&]
		{
			return ConditionOrTrue(
				(*Inventory->hamster) == false
			);
		});
		Conditions.Add(-588146277, [&]
		{
			return ConditionOrTrue(
				getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) <= -10
			);
		});
		Conditions.Add(-343334875, [&]
		{
			return ConditionOrTrue(
				getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) < 10 && getProp(getObj(FString(TEXT("Chr_Manfred"))), FString(TEXT("Morale.MoraleValue"))) > -10
			);
		});
	}
	#if !((defined(PLATFORM_PS4) && PLATFORM_PS4) || (defined(PLATFORM_PS5) && PLATFORM_PS5))
	#pragma warning(pop)
	#endif
};
