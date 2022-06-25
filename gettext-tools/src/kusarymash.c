#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <message.h>
#include <read-mo.h>
#include <write-mo.h>

#define EQ_STR( s1, s2 ) (strcmp(s1, s2) == 0)

static const struct {
    char* english;
    char* mari;
} MAP[] = {
        {"Help", "Полыш"},
        {"Copy", "Копийым ышташ"},
        {"Paste", "Шындаш"},
        {"About", "Программе нерген"},
        {"Undo", "Кораҥдаш"},
        {"Redo", "Пӧртылташ"},
        {"Preferences", "Келыштарыш-влак"},
        {"Quit", "Лекташ"},
        {"Clear", "Эрыкташ"},
        {"Close", "Петыраш"},
};

#define ARRAY_SIZE(arr) ( sizeof(arr) / sizeof(arr[0]) )

int main (int argc, char **argv) {
    if (argc != 1 + 1) {
        fprintf(stderr, "Usage: %s filename.mo\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    /* Set default value for global variables.  */
    alignment = DEFAULT_OUTPUT_ALIGNMENT;
    byteswap = 0 ^ ENDIANNESS;
    no_hash_table = false;

    message_list_ty *mlp = message_list_alloc (false);
    read_mo_file (mlp, filename);
    for (int i = 0; i < mlp->nitems; i++) {
        const char* msgid = mlp->item[i]->msgid;
        int msgid_sz = strlen(msgid) + 1;
        char msgid_norm[msgid_sz];
        int norm_pos = 0;
        for (int j = 0; j < msgid_sz; j++ ) {
            if (msgid[j] != '_')
                msgid_norm[norm_pos++] = msgid[j];
        }

        for (int j = 0; j < ARRAY_SIZE(MAP); j++ ) {
            if (EQ_STR(msgid_norm, MAP[j].english)) {
                mlp->item[i]->msgstr = MAP[j].mari;
                mlp->item[i]->msgstr_len = strlen(mlp->item[i]->msgstr) + 1;
                printf ("%s\t%s\n", msgid, msgid_norm);
                printf ("--------> %s\n", mlp->item[i]->msgstr);
            }
        }
    }

    msgdomain_write_mo (mlp, "123", filename);
    return 0;
}

