#define INDENT "                               "

#define BACK1   "\r"
#define BACK2   BACK1 BACK1
#define BACK4   BACK2 BACK2
#define BACK8   BACK4 BACK4
#define BACK16  BACK8 BACK8
#define BACK32  BACK16 BACK16
#define BACK64  BACK32 BACK32
#define BACK128 BACK64 BACK64
#define BACK256 BACK128 BACK128
#define BACK512 BACK256 BACK256

#define print(...)                                                                                 \
    printf(INDENT __VA_ARGS__);                                                                    \
    printf("\n")

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

#define reset       "\e[0m"
#define COLOR_RESET "\e[0m"

#define clear                                                                                      \
    printf("\e[1;1H\e[2J");                                                                        \
    printf(reset)

#define gun                                                                                        \
    printf("██                                                                          \n\
████                                                                        \n\
██░░████████████████████████████████████████████████████        ██████      \n\
██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██  ████░░░░░░██    \n\
██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████████████░░██░░░░░░████      \n\
  ██░░██████████████████████████████░░░░░░██░░░░        ██░░██░░██          \n\
    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██░░░░████████████████░░░░██            \n\
      ██████████████████████████████▒▒██░░██░░░░        ██░░░░░░██          \n\
                                    ██▒▒██████████████████░░░░██▓▓██        \n\
                                      ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████▓▓▓▓▓▓██      \n\
                                        ██████████████████▓▓▓▓▓▓▓▓▓▓██      \n\
                                                        ██▓▓▓▓▓▓▓▓▓▓██      \n\
                                                          ██▓▓▓▓▓▓▓▓▓▓██    \n\
                                                          ██▓▓▓▓▓▓▓▓▓▓██    \n\
                                                          ██▓▓▓▓▓▓▓▓▓▓██    \n\
                                                            ██▓▓▓▓▓▓▓▓▓▓██  \n\
                                                            ██▓▓▓▓▓▓▓▓▓▓██  \n\
                                                              ██▓▓▓▓▓▓▓▓██  \n\
                                                              ██▓▓▓▓▓▓▓▓▓▓██\n\
                                                              ██▓▓▓▓▓▓▓▓▓▓██\n\
                                                                ██████████  \n\
")