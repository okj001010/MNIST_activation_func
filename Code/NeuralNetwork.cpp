#include "NeuralNetwork.h"
#include "Matrix.h"
#include <bits/stdc++.h>
#include <random>

double NeuralNetwork::GetFunc(double val,int id)
{
    if(id == 1)
        return 1 / (1 + exp(-1*val));      // 1 / (1+e^-x)
    else
        return 0;
}
double NeuralNetwork::GetSumFunc(double val)
{
    int i;
    double sum = 0;
    for(i=1;i<=funccnt;i++)
        sum += GetFunc(val,i);
    return sum;
}

Matrix NeuralNetwork::GetSumFunc(Matrix mat)
{
    int i,j,row = mat.row, column = mat.column;

    for(i=0;i<row;i++)
        for(j=0;j<column;j++)
            mat.SetElem(i,j,GetSumFunc(mat.GetElem(i,j)));

    return mat;
}

double NeuralNetwork::GetFuncDeriv(double val,int id)
{
    if(id == 1)
        return 1;
    else
        return 0;
}
double NeuralNetwork::GetSumFuncDeriv(double val)
{
    int i;
    double sum = 0;
    for(i=1;i<=funccnt;i++)
        sum += GetFuncDeriv(val,i);
    return sum;
}

Matrix NeuralNetwork::GetSumFuncDeriv(Matrix mat)
{
    int i,j,row = mat.row, column = mat.column;

    for(i=0;i<row;i++)
        for(j=0;j<column;j++)
            mat.SetElem(i,j,GetSumFuncDeriv(mat.GetElem(i,j)));

    return mat;
}

double NeuralNetwork::GetRand(double min_val,double max_val)
{
    int rand_num = distribution(generator);
    double val = min_val + ((double)rand_num / 100000)*(max_val-min_val);
    return val;
}

NeuralNetwork::NeuralNetwork(int input_node,int hidden_node,int output_node,double learn_rate)
    : wih(hidden_node,input_node), who(output_node,hidden_node),distribution(1,100000)
{
    funccnt = 1;

    inodes = input_node;
    hnodes = hidden_node;
    onodes = output_node;

    lr = learn_rate;

    for(int i=0;i<inodes*hnodes;i++)
    {
        wih.SetElem(i/hnodes,i%hnodes,GetRand());
    }

    for(int i=0;i<hnodes*onodes;i++)
    {
        who.SetElem(i/onodes,i%onodes,GetRand());
    }
}

pair<int*,int> NeuralNetwork::ReadFile(FILE* stream)
{
    int sum = 0;
    vector<int> ans;
    char buf;
    while(true)
    {
        buf = fgetc(stream);
        if(buf == '\n')
            break;
        else if (buf == ',')
        {
            ans.push_back(sum);
            sum = 0;
        }
        else
        {
            buf -= '0';
            sum *= 10;
            sum += buf;
        }
    }
    int *arr = new int [ans.size()];
    for(int i=0;i<ans.size();i++)
        arr[i] = ans[i];
    return std::make_pair(arr,ans.size());
}

void NeuralNetwork::Train(int *input, int ans)
{
    Matrix inputs(inodes,1);
    double val;
    for(int i=0;i<inodes;i++)
    {
        val = (double)input[i] / 255;
        inputs.SetElem(i,0,val);
    }
    Matrix targets(onodes,1);
    for(int i=0;i<onodes;i++)
    {
        if(ans == i)
            targets.SetElem(i,0,1.00);
        else
            targets.SetElem(i,0,0.01);
    }
    // inputs, targets
    printf("Initial Seting Done...\n");

    Matrix hidden_inputs = wih*inputs;
    Matrix hidden_outputs(hnodes,1);
    for(int i=0;i<hnodes;i++)
        hidden_outputs.SetElem(0,i,GetSumFunc(hidden_inputs.GetElem(0,i)));
    // inputs, targets, hidden_inputs, hidden_outputs

    printf("Hidden Layer Done...\n");
    Matrix final_inputs = who*hidden_outputs;

    Matrix final_outputs(onodes,1);
    for(int i=0;i<onodes;i++)
        final_outputs.SetElem(0,i,GetSumFunc(final_inputs.GetElem(0,i)));
    // inputs, targets, hidden_inputs, hidden_outputs, final_inputs, final_outputs
    printf("Output Layer Done...\n");

    Matrix output_errors = targets - final_outputs;
    Matrix whot = who.Transpose();
    Matrix hidden_errors = whot * output_errors;
    // inputs, targets, hidden_inputs, hidden_outputs, final_inputs, final_outputs, output_errors, hidden_errors

    printf("Error Done...\n");

    Matrix whoupdate(onodes,1);
    for(int i=0;i<onodes;i++)
        whoupdate.SetElem(0,i,output_errors.GetElem(0,i) * GetSumFuncDeriv(final_outputs.GetElem(0,i)));
    Matrix t_hidden_outputs(1,hnodes);
    for(int i=0;i<hnodes;i++)
        t_hidden_outputs.SetElem(i,0,hidden_outputs.GetElem(0,i));
    Matrix whofix(onodes,hnodes);
    for(int y=0;y<onodes;y++)
        for(int x=0;x<hnodes;x++)
            whofix.SetElem(x,y,lr*whoupdate.GetElem(y,0)*t_hidden_outputs.GetElem(0,x));

    who  = who + whofix;
    //=====================================

    Matrix wihupdate(hnodes,1);
    for(int i=0;i<hnodes;i++)
        wihupdate.SetElem(0,i,hidden_errors.GetElem(0,i) * GetSumFuncDeriv(hidden_outputs.GetElem(0,i)));
    Matrix t_inputs(1,hnodes);
    for(int i=0;i<inodes;i++)
        t_inputs.SetElem(i,0,inputs.GetElem(0,i));h
    Matrix wihfix(hnodes,inodes);
    for(int y=0;y<hnodes;y++)
        for(int x=0;x<inodes;x++)
            wihfix.SetElem(x,y,lr*wihupdate.GetElem(y,0)*t_inputs.GetElem(0,x));

    wih = wih + wihfix;
}
