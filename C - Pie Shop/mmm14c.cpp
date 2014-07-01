#define NDEBUG

#include <iostream>

#include "grader.h"

void judge(int argc, char** argv)
{
    Grader G(argc, argv);
    unsigned int max_sum_correct = G.next_uint_correct();
    unsigned int ways_correct = G.next_uint_correct();
    unsigned int max_sum_progout = G.next_uint_progout();
    unsigned int ways_progout = G.next_uint_progout();
    int score = 0;
    if (max_sum_progout == max_sum_correct)
    {
        score += 4;
        if (ways_correct == ways_progout)
            score += 6;
    }
    G.finalize();
    std::cout << score << '\n' << 10 << std::endl;
}

int main(int argc, char** argv)
{
    if (argc <= 4) {
        static char *_argv[5];
        _argv[0] = argv[0];
        _argv[1] = const_cast<char*>("correct.txt");
        _argv[2] = const_cast<char*>("progout.txt");
        _argv[3] = const_cast<char*>("input.txt");
        _argv[4] = const_cast<char*>("source.txt");
        argv = _argv;
    }
    try {
        judge(argc, argv);
#ifndef NDEBUG
    } catch (Grader::BadFormatException) {
        std::cerr << "Bad Format\n";
        return 1;
    } catch (Grader::BadLengthException) {
        std::cerr << "Bad Length\n";
        return 1;
    } catch (Grader::MismatchLineException) {
        std::cerr << "Mismatch Line\n";
        return 1;
    } catch (WrongAnswerException e) {
        std::cerr << "Wrong Answer\n";
        std::cerr << e.msg << '\n';
        return 1;
#endif
    } catch (...) {
        return 1;
    }
    return 0;
}
