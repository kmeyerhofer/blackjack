#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "pcg_basic.c"

struct user {
  char name[50];
  int handTotal;
  int wins;
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
int setPlayerName(struct user *);
int setCardName(struct card *, int);
int setCardValue(struct card *, int);
int setCardSuit(struct card *, int);
int addCard(struct card *, struct user *);
int traverseUserHand(struct user *, char);
void displayUserHand(struct user *);
int calcUserHand(struct user *);
void displayFirstDealerHand(struct user *);
int blackjack(int, int);
int playAgain(void);
void resetPlayerData(struct user *, struct user *);
void startGame(struct user *, struct user *);
void gameCalcHands(struct user *, struct user *);
int blackjackCheck(int);
void increaseWins(struct user *);
int playerTurn(struct user *, struct user *);
void dealerTurn(struct user *, struct user *);
int hitOrStay(void);
int calculateWinner(struct user *, struct user *);
void displayScore(struct user *, struct user *);
int modifiedAceTotal(struct user *);


char names[13][6] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
int deck[4][13];
pcg32_random_t rng1, rng2; // Creates 2 separate RNGs


int main() {
  // Seeds 2 separate RNGs
  pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);
  pcg32_srandom_r(&rng2, time(NULL), (intptr_t)&rng2);
  printf("Welcome to Blackjack!\n\n");
  gameLoop();
  printf("Thanks for playing Blackjack!\n");
  return 0;
}

int gameLoop() {
  static struct user player;
  static struct user dealer;
  strcpy(dealer.name, "Dealer");
  setPlayerName(&player);
  player.wins = 0;
  dealer.wins = 0;

  do {
    startGame(&player, &dealer);
    gameCalcHands(&player, &dealer);
  } while (playAgain() == 1);
  printf("Final Score:\n");
  displayScore(&player, &dealer);
  return 0;
}

void displayScore(struct user *player, struct user *dealer) {
  printf("%s's wins: %d \t%s's wins: %d\n", player->name, player->wins, dealer->name, dealer->wins);
}

int blackjackCheck(int num) {
  if (num == 21) {
    return 1;
  } else {
    return 0;
  }
}

void gameCalcHands(struct user *player, struct user *dealer) {
  calcUserHand(player);
  calcUserHand(dealer);
  int playerBlackjack = blackjackCheck(player->handTotal);
  int dealerBlackjack = blackjackCheck(dealer->handTotal);
  if (playerBlackjack && dealerBlackjack) {
    // push
    displayUserHand(dealer);
    printf("Hand total: %d\n\n", dealer->handTotal);
    printf("Push.\n");
  } else if (!playerBlackjack && dealerBlackjack) {
    // dealer wins
    displayUserHand(dealer);
    increaseWins(dealer);
    // displayMatchWinner(dealer, "Dealer has 21");
    printf("%s has 21! %s wins.\n", dealer->name, dealer->name);
  } else if (playerBlackjack && !dealerBlackjack) {
    // dealer turn
    dealerTurn(player, dealer);
  } else if (!playerBlackjack && !dealerBlackjack) {
    // player turn
    playerTurn(player, dealer);
  }
}

int calculateWinner(struct user *player, struct user *dealer) {
  calcUserHand(player);
  calcUserHand(dealer);
  int playerTotal = player->handTotal;
  int dealerTotal = dealer->handTotal;
  if (playerTotal == dealerTotal) {
    printf("Push\n\n");
  } else if (dealerTotal > 21) {
    printf("Dealer Busts, %s wins!\n\n", player->name);
    increaseWins(player);
  } else if (playerTotal > dealerTotal) {
    printf("%s Wins! %i to %i\n\n", player->name, playerTotal, dealerTotal);
    increaseWins(player);
  } else if (playerTotal < dealerTotal && dealerTotal <= 21) {
    printf("%s Wins! %i to %i\n\n", dealer->name, dealerTotal, playerTotal);
    increaseWins(dealer);
  }
  return 0;
}

void dealerTurn(struct user *player, struct user *dealer) {
  while (dealer->handTotal < 17) {
    dealCard(dealer, 1);
    calcUserHand(dealer);
  }
  printf("\n%s's hand:\n", dealer->name);
  displayUserHand(dealer);
  printf("Hand total: %d\n\n", dealer->handTotal);
  if (dealer->handTotal > 21) {
    printf("%s busts, %s wins!\n\n", dealer->name, player->name);
    increaseWins(player);
  } else {
    calculateWinner(player, dealer);
  }
}

int playerTurn(struct user *player, struct user *dealer) {
  while (hitOrStay() == 1) {
    // hit
    dealCard(player, 1);
    calcUserHand(player);
    printf("\n%s's hand:\n", player->name);
    displayUserHand(player);
    printf("Hand total: %d\n\n", player->handTotal);
    if (player->handTotal > 21) {
      printf("%s Busts, %s wins!\n\n", player->name, dealer->name);
      increaseWins(dealer);
      return 0;
    } else if (player->handTotal == 21) {
      dealerTurn(player, dealer);
      return 0;
    }
  }
  // stay
  dealerTurn(player, dealer);
}

int hitOrStay(void) {
  printf("Would you like to (h)it or (s)tay?\n");
  while (1) {
    int c, answer;
    c = fgetc(stdin);
    answer = c;
    while (c != '\n' && c != EOF) {
      c = fgetc(stdin);
    }
    if (answer == 'h') {
      return 1;
    } else if (answer == 's') {
      return 0;
    }
    printf("Please answer (h)it or (s)tay:\n");
  }
}

void increaseWins(struct user *u) {
  u->wins++;
}

void startGame(struct user *player, struct user *dealer) {
  resetPlayerData(player, dealer);
  createDeck(deck);
  dealCard(player, 2);
  dealCard(dealer, 2);
  calcUserHand(player);
  printf("\n----------------------------------\n\n");
  displayScore(player, dealer);
  printf("\n%s's hand:\n", player->name);
  displayUserHand(player);
  printf("Hand total: %d\n\n", player->handTotal);
  calcUserHand(dealer);
  printf("%s's hand:\n", dealer->name);
  displayFirstDealerHand(dealer);
}

void resetPlayerData(struct user *player, struct user *dealer) {
  player->hand = 0;
  player->handTotal = 0;
  dealer->hand = 0;
  dealer->handTotal = 0;
}

int playAgain(void) {
  printf("Would you like to play again? (y/n)\n");
  while (1) {
    int c, answer;
    c = fgetc(stdin);
    answer = c;
    while (c != '\n' && c != EOF) {
      c = fgetc(stdin);
    }
    if (answer == 'y') {
      return 1;
    } else if (answer == 'n') {
      return 0;
    }
    printf("Please answer y or n:\n");
  }
}

void displayFirstDealerHand(struct user *dealer) {
  int cardValue = dealer->hand->value;
  printf("(%d) %s of %s\n", cardValue, dealer->hand->name, dealer->hand->suit);
  printf("And a face down card\n");
  printf("Card total: %d\n\n", cardValue);
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

int calcUserHand(struct user *u) {
  char calc = 'c';
  char includeAce = 'i';
  int userTotal = traverseUserHand(u, calc);
  if (userTotal > 21 && traverseUserHand(u, includeAce) > 0) {
    modifiedAceTotal(u);
  }
  return u->handTotal = traverseUserHand(u, calc);
}

int modifiedAceTotal(struct user *u) {
  char ace = 'a';
  traverseUserHand(u, ace);
  return 0;
};

void displayUserHand(struct user *u) {
  char display = 'd';
  traverseUserHand(u, display);
}

int traverseUserHand(struct user *u, char action) {
  /* 'd' for display, 'c' for calculate values,
     'a' for modify ace value, 'i' for include unmodified aces */
  int value = 0;
  if (u != 0) {
    struct card *traverser;
    traverser = u->hand;
      while (traverser != 0) {
        switch (action) {
          case 'a':
            if (traverser->value == 11) {
              traverser->value = 1;
              return 1;
            }
          case 'c':
            value += traverser->value;
            break;
          case 'd':
            printf("(%d) %s of %s\n", traverser->value, traverser->name, traverser->suit);
            break;
          case 'i':
            if (traverser->value == 11) {
              value += 1;
            }
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

int setPlayerName(struct user *player) {
  // get name from user
  char name[50];
  printf("Enter your name:\n");
  fgets(name, 50,stdin);
  name[strcspn(name, "\n")] = 0; // removes newline
  strcpy(player->name, name);
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
