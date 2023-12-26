#ifndef EFP_POXIS_HPP_
#define EFP_POXIS_HPP_

#include <signal.h>

namespace efp
{
    namespace poxis
    {
        inline void block_signal()
        {
            sigset_t set;
            sigfillset(&set);
            pthread_sigmask(SIG_BLOCK, &set, NULL);
        }

        inline void handle_signal(void (*handler)(int))
        {
            // Unblocking signals in the main thread
            sigset_t set;
            sigemptyset(&set);
            pthread_sigmask(SIG_SETMASK, &set, NULL);

            // Register signal handler for main thread
            signal(SIGINT, handler);
        }
    }
}

#endif