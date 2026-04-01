#pragma once
#include "Socket.hpp"
#include "Protocol.hpp"

class Netcal
{
public:
    Netcal()
    {
    }
    Response solve(Request &reqe)
    {
        Response rese(0, 0);

        switch (reqe.Getchar())
        {
        case '+':
            rese.Setresult(reqe.Getx() + reqe.Gety());
            break;
        case '-':
            rese.Setresult(reqe.Getx() - reqe.Gety());
            break;
        case '%':
            rese.Setresult(reqe.Getx() % reqe.Gety());
            break;
        case '/':
            if (reqe.Gety() == 0)
            {
                rese.Setcode(1);
            }
            else
            {
                rese.Setresult(reqe.Getx() / reqe.Gety());
                break;
            }
        }
    }

private:
};