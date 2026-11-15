/*
 *    George K Oliynyk
 *    Stevens Rocketry USLI 2027
 */

/* This code is heavily based upon
 * https://sources.debian.org/data/main/s/systemd-udeb/260-2/man/notify-selfcontained-example.c?pow_referer=,
 * and should be credited as such.
 */

#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <systemd/sd-daemon.h>

#include "notify.h"

static volatile sig_atomic_t reloading = 0;
static volatile sig_atomic_t terminating = 0;

static void signal_handler(int sig) {
  if (sig == SIGHUP)
    reloading = 1;
  else if (sig == SIGINT || sig == SIGTERM)
    terminating = 1;
}

int main(void) {
  struct sigaction sa = {
    .sa_handler = signal_handler,
    .sa_flags = SA_RESTART,
  };
  int r;

  /* Setup signal handlers */
  sigemptyset(&sa.sa_mask);
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);

  fprintf(stderr, "Starting up...\n");

  r = sd_notify(0, "READY=1");
  if (r < 0) {
    fprintf(stderr, "Failed to notify readiness to $NOTIFY_SOCKET: %s\n", strerror(-r));
    return EXIT_FAILURE;
  }

  fprintf(stderr, "Testing service running.\n");

  while (!terminating) {
    if (reloading) {
      reloading = false;

      r = notify_reloading();
      if (r < 0) {
        fprintf(stderr, "Failed to notify reloading to $NOTIFY_SOCKET: %s\n", strerror(-r));
        return EXIT_FAILURE;
      }

      fprintf(stderr, "Reloading to $NOTIFY_SOCKET...\n");

      r = sd_notify(0, "READY=1");
      if (r < 0) {
        fprintf(stderr, "Failed to notify readiness to $NOTIFY_SOCKET: %s\n", strerror(-r));
        return EXIT_FAILURE;
      }
    }

    /* do nothing */

    sleep(5);
  }

  r = sd_notify(0, "STOPPING=1");
  if (r < 0) {
    fprintf(stderr, "Failed to report termination to $NOTIFY_SOCKET: %s\n", strerror(-r));
    return EXIT_FAILURE;
  }

  fprintf(stderr, "Shutting down...\n");

  return EXIT_SUCCESS;
}