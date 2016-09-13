#include <stdio.h>
#include <string.h>

#include "genUse.h"

using namespace gen;
int main(int argc, char** argv)
{
    FILE* src = 0;
    FILE* puke = 0;
    long long p = 0;
    size_t len = 0;
    size_t flen = 0;
    size_t plen = 0;
    size_t step = 1;
    size_t nr_len;
    char* pnr;
    char* pname = 0;
    char* fname = 0;
    long long val;
    const char* a1 = argv[1];
    unsigned char* buf = 0;

    if(argc < 3)
    {
        printf("Usage: %s <step> <filename-list>\r\n", argv[0]);
        return -1;
    }

    fname = argv[2];
    flen = strlen(fname);
    src = fopen(fname, "rb");
    step = strToInt(a1);

    if(!src)
    {
        printf("Usage: %s <filename-list>\r\n", argv[0]);
        return -1;
    }

    fseek(src, 0, SEEK_END);
    len = ftell(src);
    rewind(src);
    buf = (unsigned char*)malloc(len * sizeof(unsigned char));
    if (!buf)
    {
        fclose(src);
        return -1;
    }

    if(fread(buf, len, 1, src) != 1)
    {
        free(buf);
        fclose(src);
        return -1;
    }

    printf("<Info> Source \"%s\" has size %u, stepping = %uByte\r\n", fname, len, step);
    for(p = step; p < len; p+=step)
    {
        val = p;
        //rewind(src);
        nr_len = 1;
        while(val > 0)
        {
            val /= 10;
            if(val > 0)
                ++nr_len;
        }
        val = p;
        pnr = (char*)malloc((nr_len+1) * sizeof(char));
        pnr = intToStr(pnr, val);
        //printf("<Info> Nummer %s\r\n", pnr);
        if(pnr)
        {
            nr_len = strlen(pnr);
            plen = flen + 1 + nr_len;

            pname = (char*)malloc(plen+1 * sizeof(char));
            if(pname)
            {
                sprintf(pname, "%s#%s", pnr, fname);

                puke = fopen(pname, "wb");
                if(puke)
                {
                    printf("Writing file [%ldKiB], name=<%s>\r\n", (long)p/1024, pname);
                    if(fwrite(buf, p, 1, puke) != 1)
                        printf("<Error> fwrite(buf, p, 1, puke)\r\n");
                    fclose(puke);
                }
                free(pname);
            }

            free(pnr);
        }
    }

    free(buf);
    fclose(src);
    return 0;

}
