// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstSaveGame.h"

UFirstSaveGame::UFirstSaveGame()
{
	PlayerName = TEXT("DefaultName");
	UserIndex = 0;
	CharacterStats.WeaponName = TEXT("");
	CharacterStats.LevelName = TEXT("");

	CharacterStats.Health;
	CharacterStats.MaxHealth;
	CharacterStats.Stamina;
	CharacterStats.MaxStamina;
	CharacterStats.Coins;
}
