#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "pcg_basic.c"

struct user {
  char name[50];
  int handTotal;
  struct card *hand;
} user;

struct card {
  int value;
  char suit[9];
  char name[6];
  struct card *next;
} card;

int gameLoop(void);
int createDeck(int deck[4][13]);
void dealCard(struct user *, int);
int setUserName(struct user *);
int setCardName(struct card *, int);
int setCardValue(struct card *, int);
int setCardSuit(struct card *, int);
int addCard(struct card *, struct user *);
int traverseUserHand(struct user *, char);
void displayUserHand(struct user *);
int calcUserHand(struct user *);


char names[13][6] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
int deck[4][13];
pcg32_random_t rng1, rng2; // Creates 2 separate RNGs


int main() {
  // Seeds 2 separate RNGs
  pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);
  pcg32_srandom_r(&rng2, time(NULL), (intptr_t)&rng2);
  printf("Welcome to Blackjack!\n\n");
  gameLoop();
  return 0;
}

int gameLoop() {
  static struct user player;
  static struct user dealer;
  strcpy(dealer.name, "Dealer");
  setUserName(&player);

  //loop goes here
  createDeck(deck);
  dealCard(&player, 2);
  dealCard(&dealer, 2);
  printf("%s's hand:\n", player.name);
  char add = 'a';
  displayUserHand(&player);
  player.handTotal = calcUserHand(&player);
  printf("Hand total: %d\n\n", player.handTotal);
  printf("%s's hand:\n", dealer.name);
  displayUserHand(&dealer); // Dealer gets special output and total
  dealer.handTotal = calcUserHand(&dealer);
  printf("Hand total: %d\n", dealer.handTotal);

  return 0;
}

void dealCard(struct user *player, int times) {
  int i;
  int suitIndex;
  int cardIndex;
  for (i = 0; i < times; i++) {
    struct card *userCard;
    userCard = (struct card *) malloc(sizeof(struct card));
    userCard->next = 0;
    do {
      suitIndex = pcg32_boundedrand_r(&rng1, 4);
      cardIndex = pcg32_boundedrand_r(&rng2, 13);
    } while (!deck[suitIndex][cardIndex]);
    deck[suitIndex][cardIndex] = 0;
    setCardSuit(userCard, suitIndex);
    setCardName(userCard, cardIndex);
    setCardValue(userCard, cardIndex);
    addCard(userCard, player);
  }
}

int calcUserHand(struct user *player) {
  char add = 'a';
  return traverseUserHand(player, add);
}

void displayUserHand(struct user *player) {
  char display = 'd';
  traverseUserHand(player, display);
}

int traverseUserHand(struct user *player, char action) {
  // 'd' for display, 'a' for add values
  int value = 0;
  if (player != 0) {
    struct card *traverser;
    traverser = player->hand;
      while (traverser != 0) {
        switch (action) {
          case 'd':
            printf("(%d) %s of %s\n", traverser->value, traverser->name, traverser->suit);
            break;
            case 'a':
            value += traverser->value;
            break;
          }
          traverser = traverser->next;
        }
      }
  return value;
}

int addCard(struct card *userCard, struct user *player) {
  struct card *traverserCard;
  if (player != 0) {
    if (player->hand != 0) {
      traverserCard = player->hand;
      while (traverserCard->next != 0) {
        traverserCard = traverserCard->next;
      }
      traverserCard->next = userCard;
    } else {
      player->hand = userCard;
    }
  }
  return 0;
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

int setUserName(struct user *player) {
  // get name from user
  strcpy(player->name, "Player");
}

int setCardSuit(struct card *newCard, int index) {
  switch (index) {
    case 0:
      strcpy(newCard->suit, "Diamonds");
      break;
    case 1:
      strcpy(newCard->suit, "Hearts");
      break;
    case 2:
      strcpy(newCard->suit, "Spades");
      break;
    case 3:
      strcpy(newCard->suit, "Clubs");
      break;
  }
  return 0;
}

int setCardValue(struct card *newCard, int cardIndex) {
  if (cardIndex > 9) {
    newCard->value = 10;
  } else if (cardIndex == 0) {
    newCard->value = 11;
  } else {
    newCard->value = cardIndex + 1;
  }
  return 0;
}

int setCardName(struct card *newCard, int cardIndex) {
  strcpy(newCard->name, names[cardIndex]);
  return 0;
}
