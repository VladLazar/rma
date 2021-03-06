/**
 * Copyright (C) 2018 Dean De Leo, email: dleo[at]cwi.nl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <cassert>
#include <chrono>
#include <ratio>

class Timer{
public:
    typedef typename std::chrono::steady_clock clock;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer& timer) = delete;

private:
    clock::duration base; // saved durations from previous runs
    clock::time_point t0; // starting time
    bool isRunning;

    clock::duration length() const {
        return base + ((isRunning) ? clock::now() - t0 : clock::duration::zero());
    }

    template<typename Type, typename Period>
    Type my_cast() const{
        using D = std::chrono::duration<Type, Period>;
        return std::chrono::duration_cast<D>(length()).count();
    }


public:

    Timer(bool start_timer = false) : base(clock::duration::zero()), t0(), isRunning(false){
        if(start_timer) start();
    }

    void start(){
        t0 = clock::now();
        isRunning = true;
    }

    void stop(){
        assert(isRunning);
        auto t1 = clock::now();
        base += t1 - t0;
        isRunning = false;
    }

    void reset(bool restart = false){
        base = clock::duration::zero();
        isRunning = false;
        if(restart) start();
    }

    bool is_running(){
        return isRunning;
    }

    template<typename T = clock::rep> // int64_t on my current system
    T milliseconds() const {
        return my_cast<T, std::milli>();
    }

    template<typename T = clock::rep>
    T microseconds() const {
        return my_cast<T, std::micro>();
    }

    template<typename T = clock::rep>
    T nanoseconds() const {
        return my_cast<T, std::nano>();
    }

    template<typename T = clock::rep>
    T seconds() const{
        return my_cast<T, std::ratio<1>>();
    }
};



#endif /* TIMER_HPP_ */
