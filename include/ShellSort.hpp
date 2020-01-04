#pragma once

#include "Sort.hpp"

class ShellSort : public Sort
{
public:
    ShellSort(std::string const &sortName);

private:
    void Step() override;
};