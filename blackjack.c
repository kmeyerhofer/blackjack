#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
// #include "rng.c"
#include "pcg_basic.c"

struct card {
  int value;
  char suit[9];
  char name[6];
  struct card *next;
} card;

// int rand(void);
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
  // printf("before createDeck\n");
  createDeck(deck);
  // printf("after createDeck\n");

  // printf("before struct initialization\n");
  struct card playerHand;
  //struct *ptrPlayerHand = playerHand;
  // printf("after struct initialization\n");
  // playerHand = malloc(sizeof(struct card));
  // playerHand->next = 0;
  // printf("before dealCard\n");
  dealCard(&playerHand, 1);
  // printf("after dealCard\n");
  printf("Here is your hand:\n");
  printf("Suit within gameLoop: %s\n", playerHand.suit);
  // playerHand.value = 10;
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
      // suit = randLimit(3);
      // card = randLimit(12);
      //
      suitIndex = pcg32_boundedrand_r(&rng1, 4);
      cardIndex = pcg32_boundedrand_r(&rng2, 13);


    } while (!deck[suitIndex][cardIndex]);
    deck[suitIndex][cardIndex] = 0;
    printf("dealCard: suitIndex: %d, cardIndex: %d\n", suitIndex, cardIndex);
    // printf("pcg32_boundedrand_r(&rng1, 15) call: %d\n", pcg32_boundedrand_r(&rng1, 15));
    // printf("pcg32_boundedrand_r(&rng2, 13) call: %d\n", pcg32_boundedrand_r(&rng2, 13));
    // printf("randLimit(3) call: %d\n", randLimit(3));

    setCardSuit(hand, suitIndex);
    setCardName(hand, cardIndex);
    setCardValue(hand, cardIndex);
  }
  // printf("after assigning to *hand\n");

  // printf("Within dealCard: %s\n", hand->suit);
  // player = malloc(sizeof(*player));
  // player->next = 0;
}

int createDeck(int deck[4][13]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 13; j++) {
      deck[i][j] = j + 1;
    }
  }
  // printf("Deck 2, 2: %i\n", deck[2][2]);

  return 0;
}

int randLimit(int max) {
  // double maxNum = genrand();
  // printf("maxNum (genrand()): %d\n", maxNum);
  // int divisor = maxNum / (max + 1);
  // int num;
  // do {
    // num =  maxNum / divisor;
  // } while (num > max);
  // printf("int num before int typecast: %i\n", num);
  // return (int)num;
  // printf("int num after int typecast: %d\n", num);
  // double random = genrand();
  // if (random < 0) {
  //   random *= -1;
  // }
  // printf("'random' value: %d\n", random);
  // num = ((int)random % max) + 1;
  // printf("'num' value: %d\n", num);
  // return num;

  // return random;
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
