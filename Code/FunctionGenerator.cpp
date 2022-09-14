#ifndef FUNCTIONGENERATOR_CPP
#define FUNCTIONGENERATOR_CPP

#include <bits/stdc++.h>

namespace FuncGen
{
	typedef double (*FuncPtr)(double);

    double Func1(double val)
	{
	    return 1/(1 + std::exp(-1 * val));
    }

	FuncPtr funcsource[1] = {Func1};

    double Generating(double val, double *weight, int w_len)
	{
		int len = 1;
		double sum = 0;

		int i;
		for(i=0;i<w_len;i++)
        {
            if(i >= len)    break;

            sum += weight[i] * funcsource[i](val);
        }

        return sum;
	}
}

#endif
