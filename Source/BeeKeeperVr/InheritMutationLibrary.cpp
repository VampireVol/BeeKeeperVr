// Fill out your copyright notice in the Description page of Project Settings.


#include "InheritMutationLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Templates/UnrealTemplate.h"
#include "Containers/Map.h"

const float MUTATION_GEN_CHANCE = 0.05f;
const float MUTATION_SPECIES_CHANCE = 0.4f;
const float MUTATION_SPECIES_CHANCE_PARENTS_EQUAL = 0.1f;

const float MUTATION_TIER_DOWN_TWO_CHANCE = 0.1f;
const float MUTATION_TIER_DOWN_ONE_CHANCE = 0.3f;
const float MUTATION_TIER_NO_CHANGE_CHANCE = 0.7f;
const float MUTATION_TIER_UP_ONE_CHANCE = 0.9f;
const float MUTATION_TIER_UP_TWO_CHANCE = 1.0f;

const int32 SPEED_GENS_COUNT = 8;
const int32 FERTILITRY_GENS_COUNT = 10;

static TMultiMap<int32, TEnumAsByte<Species>> Tiers {
  {1, Species::Meadow},
  {1, Species::Forest},
  {1, Species::River},
  {2, Species::Plant},
  {2, Species::Mushroom},
  {2, Species::Berry},
  {2, Species::Woody},
  {3, Species::Potato},
  {3, Species::Wheat},
  {3, Species::Grape},
  {3, Species::Tomato},
  {4, Species::Flour},
  {4, Species::Plank},
  {4, Species::Paper},
  {5, Species::Bread},
  {5, Species::Wine},
  {5, Species::Boxed},
  {5, Species::Ketchup},
};

static bool random(float less)
{
  return FMath::FRandRange(0.f, 1.f) < less;
}

//https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit
static void set_bit(int32 &num, int32 pos)
{
  num |= 1UL << pos;
}

static void clear_bit(int32 &num, int32 pos)
{
  num &= ~(1UL << pos);
}

static void flip_bit(int32 &num, int32 pos)
{
  num ^= 1UL << pos;
}

static bool get_bit(int32 num, int32 pos)
{
  return (num >> pos) & 1U;
}

static int32 find_tier(TEnumAsByte<Species> s)
{
  return *Tiers.FindKey(s);
}

static void mutate(TEnumAsByte<Species> &s, float mutation_chance)
{
  if (random(mutation_chance))
  {
    int32 tier = find_tier(s);
    float chance = FMath::FRandRange(0.f, 1.f);
    if (chance < MUTATION_TIER_DOWN_TWO_CHANCE)
      tier -= 2;
    else if (chance < MUTATION_TIER_DOWN_ONE_CHANCE)
      --tier;
    else if (chance > MUTATION_TIER_NO_CHANGE_CHANCE &&
             chance < MUTATION_TIER_UP_ONE_CHANCE)
      ++tier;
    else
      tier += 2;

    if (tier < 0)
      tier = 0;
    TArray<TEnumAsByte<Species>> species;
    Tiers.MultiFind(tier, species);
    s = species[FMath::RandRange(0 , species.Num() - 1)];
  }
}

static void mutate(TEnumAsByte<Species> &p1, TEnumAsByte<Species> &p2)
{
  if (random(0.5f))
    Swap(p1, p2);
  float mutChacne = 0.0f;
  if (p1 == p2)
    mutChacne = MUTATION_SPECIES_CHANCE_PARENTS_EQUAL;
  else
    mutChacne = MUTATION_SPECIES_CHANCE;
  mutate(p1, mutChacne);
  mutate(p2, mutChacne);
}

static void mutate(int32 &g, int32 gen_count)
{
  const int32 genPairs = gen_count / 2;
  for (int i = 0; i < gen_count; ++i)
  {
    if (random(MUTATION_GEN_CHANCE))
    {
      flip_bit(g, i);
    }
  }
  for (int i = 0; i < genPairs; ++i)
  {
    if (get_bit(g, i * 2) < get_bit(g, i * 2 + 1))
    {
      set_bit(g, i * 2);
      clear_bit(g, i * 2 + 1);
    }
  }
}

static int32 inherit(int32 p1, int32 p2, int32 gen_count)
{
  const int32 genPairs = gen_count / 2;
  int32 out = 0;
  for (int i = 0; i < genPairs; ++i)
  {
    bool g1 = random(0.5f) ? get_bit(p1, i * 2) : get_bit(p1, i * 2 + 1);
    bool g2 = random(0.5f) ? get_bit(p2, i * 2) : get_bit(p2, i * 2 + 1);
    if (g1 && g2)
    {
      set_bit(out, i * 2);
      set_bit(out, i * 2 + 1);
    }
    else if (g1 || g2)
      set_bit(out, i * 2);
  }
  mutate(out, gen_count);
  return out;
}

UBeeGenetic *UInheritMutationLibrary::Inherit(const UBeeGenetic *p1, const UBeeGenetic *p2)
{
  TEnumAsByte<Species> speciesP1 = random(0.5f) ? p1->Main : p1->Sec;
  TEnumAsByte<Species> speciesP2 = random(0.5f) ? p2->Main : p2->Sec;
  mutate(speciesP1, speciesP2);
  int32 speed = inherit(p1->Speed, p2->Speed, SPEED_GENS_COUNT);
  int32 fertility = inherit(p1->Fertility, p2->Fertility, FERTILITRY_GENS_COUNT);
  return UBeeGenetic::Construct(speciesP1, speciesP2, speed, fertility);
}
