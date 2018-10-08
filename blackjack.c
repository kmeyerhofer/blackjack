#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "pcg_basic.c"

struct card {
  int value;
  char suit[9];
  char name[6];
  struct card *next;
} card;

int gameLoop(void);
int createDeck(int deck[4][13]);
void dealCard(struct card *, int);
int randLimit(int);
int setCardName(struct card *, int);
int setCardValue(struct card *, int);
int setCardSuit(struct card *, int);

int deck[4][13];
char names[13][6] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};

pcg32_random_t rng1, rng2; // Creates 2 separate RNGs


int main() {
  // Seeds 2 separate RNGs
  pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);
  pcg32_srandom_r(&rng2, time(NULL), (intptr_t)&rng2);
  printf("Welcome to Blackjack!\n");
  gameLoop();
  return 0;
}

int gameLoop() {
  createDeck(deck);
  struct card playerHand;
  dealCard(&playerHand, 1);
  printf("Here is your hand:\n");
  printf("Suit within gameLoop: %s\n", playerHand.suit);
  printf("Value within gameLoop: %d\n", playerHand.value);
  printf("Name within gameLoop: %s\n", playerHand.name);
  return 0;
}

void dealCard(struct card *hand, int times) {
  int i;
  int suitIndex;
  int cardIndex;
  for (i = 0; i < times; i++) {
    do {
      suitIndex = pcg32_boundedrand_r(&rng1, 4);
      cardIndex = pcg32_boundedrand_r(&rng2, 13);
    } while (!deck[suitIndex][cardIndex]);
    deck[suitIndex][cardIndex] = 0;
    printf("dealCard: suitIndex: %d, cardIndex: %d\n", suitIndex, cardIndex);

    setCardSuit(hand, suitIndex);
    setCardName(hand, cardIndex);
    setCardValue(hand, cardIndex);
  }
}

int createDeck(int deck[4][13]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 13; j++) {
      deck[i][j] = j + 1;
    }
  }
  return 0;
}

int setCardSuit(struct card *hand, int value) {
  switch (value) {
    case 0:
      strcpy(hand->suit, "Diamonds");
      break;
    case 1:
      strcpy(hand->suit, "Hearts");
      break;
    case 2:
      strcpy(hand->suit, "Spades");
      break;
    case 3:
      strcpy(hand->suit, "Clubs");
      break;
  }
  return 0;
}

int setCardValue(struct card *hand, int cardIndex) {
  if (cardIndex > 9) {
    hand->value = 10;
  } else if (cardIndex == 0) {
    hand->value = 11;
  } else {
    hand->value = cardIndex + 1;
  }
  return 0;
}

int setCardName(struct card *hand, int cardIndex) {
  strcpy(hand->name, names[cardIndex]);
  return 0;
}
