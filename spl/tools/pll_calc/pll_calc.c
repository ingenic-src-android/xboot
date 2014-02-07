#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double vco_min = 3e+08;
double vco_max = 1.5e+09;
double ref_min = 183105;
double ref_max = 1.5e+09;
int nr_min = 1;
int nr_max = 64;
int nf_min = 1;
int nf_max = 4096;
int no_min = 1;
int no_max = 16;
int no_even = 1;
int nb_min = 1;
int nb_max = 4096;
int max_vco = 1;
int ref_rng = 1;

char *fmt_hex (long long num, int nbits, char *d);

//char *fmt_hex (num, nbits, d)
//long long num;
//int nbits;
//char *d;
char *fmt_hex (long long num, int nbits, char *d)
{
	sprintf (d, "0x%0*llX", (nbits - 1) / 4 + 1, num);
	return (d);
}

//main (argc, argv)
//int argc;
//char **argv;
int main(int argc, char **argv)
{
	int nr, nrx, nf, nfi, no, noe, not, nor, nore, nb, first, found;
	long long nfx;
	double fin, fout, fvco;
	double val, nval, err, merr, terr;
	int x_nrx, x_no, x_nb;
	long long x_nfx;
	double x_fvco, x_err;
	char d[1024];

	terr = -1;
	while (argc > 1) {
		if (! strcmp (argv[1], "-n")) {
			max_vco = 0;
		} else if (! strcmp (argv[1], "-r")) {
			ref_rng = 0;
		} else if ((! strcmp (argv[1], "-f")) && (argc > 2)) {
			nf = atoi (argv[2]);
			if (nf > nf_min) nf_min = nf;
			++argv;
			--argc;
		} else if ((! strcmp (argv[1], "-v")) && (argc > 2)) {
			fvco = atof (argv[2]);
			if (fvco > vco_min) vco_min = fvco;
			++argv;
			--argc;
		} else {
			break;
		}
		++argv;
		--argc;
	}
	if ((argc == 3) || (argc == 4)) {
		fin = atof (argv[1]);
		fout = atof (argv[2]);
		val = fout / fin;
		if (argc == 4) {
			terr = atof (argv[3]);
			if (terr == -1) {
				terr = 1 / ((double) nf);
			}
		}
	} else {
		fprintf (stderr, "Calculates PLL settings given the input and output frequency and optional\n"
				  "error tolerance.  If max_error is specified, the optimal PLL bit settings\n"
				  "are printed that achieve an output frequency within the error tolerance,\n"
				  "otherwise a list of settings is printed using progressively larger\n"
				  "multiplication factors to reduce the error (use -1 for error within\n"
				  "resolution limit and -2 for lowest error).\n");
		fprintf (stderr, "\n");
		fprintf (stderr, "Usage: $0 [-n] [-r] [-f nf_min] [-v vco_min] fin fout [max_error]\n");
		fprintf (stderr, "\n");
		fprintf (stderr, "\t-n - do not maximize VCO frequency\n");
		fprintf (stderr, "\t-r - do not range check NR or minimum fin/NR\n");
		fprintf (stderr, "\t-f nf_min - minimum allowed NF value\n");
		fprintf (stderr, "\t-v vco_min - minimum allowed VCO frequency\n");
		exit (1);
	}

	if (fout * no_min > vco_max) {
		fprintf (stderr, "Error:  Maximum frequency exceeded.\n");
		exit (1);
	}
	if (fout * no_max < vco_min) {
		fprintf (stderr, "Error:  Minimum frequency exceeded.\n");
		exit (1);
	}

	first = 1;
	if (terr == -1) {
		printf ("NR\tNF\tOD\tNB\tFvco\t\terror\n");
		printf ("------------------------------------------------------\n");
	} else if (terr != -2) {
		first = 0;
		if (terr == 0) terr = 1e-16;
		merr = terr;
	}
	found = 0;
	for (nfi = val; nfi < nf_max; ++nfi) {
		nr = rint (((double) nfi) / val);
		if (nr == 0) continue;
		if ((ref_rng) && (nr < nr_min)) continue;
		if (fin / ((double) nr) > ref_max) continue;
		nrx = nr;
		nf = nfx = nfi;
		nval = ((double) nfx) / ((double) nr);
		if (nf == 0) nf = 1;
		err = 1 - nval / val;

		if ((first) || (fabs (err) < merr * (1 - 1e-6))) {
			not = floor (vco_max / fout);
			for (no = (not > no_max)? no_max : not; no > no_min; --no) {
				if ((no_even) && (no & 1)) {
					continue;
				}
				if ((ref_rng) && ((nr / no) < nr_min)) continue;
				if ((nr % no) == 0) break;
			}
			if ((nr % no) != 0) continue;
			nor = ((not > no_max)? no_max : not) / no;
			nore = nf_max / nf;
			if (nor > nore) nor = nore;
			if (((no_even) && (no & 1)) && (nor & 1) && (nor > 1)) --nor;
			noe = ceil (vco_min / fout);
			if (! max_vco) {
				nore = (noe - 1) / no + 1;
				if ((no_even) && (no & 1) && (nor > nore) && (nore & 1)) ++nore;
				nor = nore;
				not = 0;	/* force next if to fail */
			}
			if ((((no * nor) < (not >> 1)) || ((no * nor) < noe)) &&
					((no * nor) < (nf_max / nf))) {
				no = nf_max / nf;
				if (no > no_max) no = no_max;
				if (no > not) no = not;
				if ((no_even) && (no & 1) && (no > 1)) --no;
				nfx *= no;
				nf *= no;
			} else {
				nrx /= no;
				nfx *= nor;
				nf *= nor;
				no *= nor;
				if (no > no_max) continue;
				if ((no_even) && (no & 1) && (no > 1)) continue;
			}

			nb = rint (nfx / 2.0);
			if (nb < nb_min) nb = nb_min;
			if (nb > nb_max) continue;

			fvco = fin / ((double) nrx) * ((double) nfx);
			if (fvco < vco_min) continue;
			if (fvco > vco_max) continue;
			if (nf < nf_min) continue;
			if ((ref_rng) && (fin / ((double) nrx) < ref_min)) continue;
			if ((ref_rng) && (nrx > nr_max)) continue;

			found = 1;
			if (terr == -2) {
				x_nrx = nrx;
				x_nfx = nfx;
				x_no = no;
				x_nb = nb;
				x_fvco = fvco;
				x_err = err;
			} else if (terr != -1) {
				break;
			}
			first = 0;
			merr = fabs (err);
			if (terr == -2) continue;
			printf ("%d\t%lld\t%d\t%d\t%e\t%#+g\n",
				nrx, nfx, no, nb, fvco, err);
		}
		if (err == 0) break;
	}
	if (! found) {
		fprintf (stderr, "Error:  No workable settings found.\n");
		exit (1);
	}
	if (terr != -1) {
		if (terr == -2) {
			nrx = x_nrx;
			nfx = x_nfx;
			no = x_no;
			nb = x_nb;
			fvco = x_fvco;
			err = x_err;
		} else if (fabs (err) >= terr * (1 - 1e-6)) {
			fprintf (stderr, "Error:  No appropriate ratio found.\n");
			exit (1);
		}

		printf ("NR = %d\n", nrx);
		printf ("NF = %lld\n", nfx);
		printf ("OD = %d\n", no);
		printf ("NB = %d\n", nb);

		printf ("\n");
		printf ("Fin  = %g\n", fin);
		printf ("Fvco = %g\n", fvco);
		printf ("Fout = %g\n", fvco / no);
		printf ("error = %+g\n", err);

		printf ("\n");
		printf ("CLKR[5:0] = %s\n", fmt_hex (nrx-1, 6, d));
		printf ("CLKF[12:0] = %s\n", fmt_hex (nfx-1, 13, d));
		printf ("CLKOD[3:0] = %s\n", fmt_hex ((no-1)*1, 4, d));
		printf ("BWADJ[11:0] = %s\n", fmt_hex (nb-1, 12, d));
	}

	exit (0);
}
