#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include "kmer.h"
#include "contig.h"
#include "utils.h"

using namespace std;

uint32_t Contig::id_generator = 0;
k_t Contig::k = K;      /* TODO - Set this dynamically. */
size_t Contig::seed = 0;

Contig::Contig()
    : id(id_generator++), s(), left_ext(), right_ext()
{
}

Contig::Contig(kmer_t init_kmer)
    : id(id_generator++), s(), left_ext(), right_ext()
{
    char kmer_str[k + 1];
    kmer2str(kmer_str, init_kmer, k);
    s = kmer_str;
}

bool Contig::check_next_left_ext(base next_left_ext)
{
    return char2base(s[s.size() - k]) == next_left_ext;
}

void Contig::revcmp(void)
{
    reverse(s.begin(), s.end());
    for (string::iterator it = s.begin(); it < s.end(); it++)
        *it = inv_base(*it);

    swap(left_ext, right_ext);
    left_ext = inv_base(left_ext);
    right_ext = inv_base(right_ext);
}

void Contig::get_ext_kmer(kmer_t kmer)
{
    size_t start_idx = s.size() - (k-1);
    for (size_t i = 0; i < k - 1; i++) {
        base b = char2base(s[start_idx + i]);
        set_base(kmer, i, b);
    }
    set_base(kmer, k - 1, right_ext);
}

void Contig::fprint(FILE* outfile)
{
    fprintf(outfile, "%s", s.c_str());
}

void Contig::fprintln(FILE* outfile)
{
    fprint(outfile);
    fprintf(outfile, "\n");
}

void Contig::fprint_fasta(FILE* outfile, size_t textwidth)
{
    fprintf(outfile, ">contig %u len %lu\n", id, s.size());
    size_t i;
    for (i = 0; i < s.size() / textwidth; i++) {
        string substr = s.substr(i * textwidth, textwidth);
        fprintf(outfile, "%s\n", substr.c_str());
    }
    string substr = s.substr(i * textwidth, s.size() % textwidth);
    fprintf(outfile, "%s\n", substr.c_str());
}

void Contig::verify()
{
    for (size_t i = 0; i < s.size(); i++)
    {
        char c = s[i];
        switch (c)
        {
            case 'A':
            case 'C':
            case 'G':
            case 'T':
                break;
            default:
                panic("BAD BASE: %c\n", c);
        }
    }
}
