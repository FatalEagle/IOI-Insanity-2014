#!/usr/bin/python
#########################################################################
#                                                                       #
# Lift Control - Elevator and Passenger Simulator                       #
# Copyright (c) 2014 Alex Li <alextrovert@gmail.com>                    #
#                                                                       #
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <http://www.gnu.org/licenses/>. #
#                                                                       #
# usage: simulator.py [-h] [-i INFILE] [-cmd CMDFILE] [-log LOGFILE]    #
#   e.g. simulator.py -i lift.in -cmd lift.out -log lift.log            #
#                                                                       #
#########################################################################

import argparse
import array
import collections
import math
import sys

MAX_F = 1000
MAX_S = 20
MAX_V = 20
MAX_N = 10000
MAX_T = 100000

logtext = []

def log(text):
    global logtext
    logtext.append(text)

def simulate(fcmd, F, S, V, people):
    # inside contains those current in the elevator
    # inside[f] = list of people getting off at floor f
    inside  = [[] for i in xrange(1, F+2)]
    # waiting[f] = list of people [ID, B] waiting on floor f
    waiting = [[] for i in xrange(1, F+2)]
    # wait_lo[i] = T[i] = the time when person i started waiting
    # wait_hi[i] = the time when person i got out at their floor
    wait_lo = array.array('i', [-1]*(len(people)+1))
    wait_hi = array.array('i', [-1]*(len(people)+1))

    idx = 0 # current index in the list of people
    t = 0
    f = 1
    log('\t\tt = %-5d floor = %-5d\n' % (t, f))
    for line in fcmd:
        cmd, val = line[0], int(line[2:])
        log('%s %d:\n' % (cmd, val))
        assert cmd in ['G', 'S'], 'Command must be either S or G'
        if cmd == 'G':
            t = int(math.ceil(t + math.fabs(val - f)/V))
            f = val
        elif cmd == 'S':
            if val >= S:  # open the doors to let people out
                for ID in inside[f]:
                    log(('Passenger %d gets off (waited [%d, %d] = %d)\n'
                      % (ID, wait_lo[ID], t, t - wait_lo[ID] + 1)).rjust(70))
                    wait_hi[ID] = t
                inside[f] = []

            t += val

            # move all people up to the current time into waiting
            while idx < len(people) and people[idx][1] <= t:
                ID = people[idx][0]
                wait_lo[ID] = T = people[idx][1]
                A = people[idx][2]
                B = people[idx][3]
                waiting[A].append([ID, B])
                log(('Passenger %d now waits on floor %d (t = %d)\n'
                     % (ID, A, T)).rjust(70))
                idx += 1
            
            if val >= S:  # open the doors to let people in
                for p in waiting[f]:
                    log(('Passenger %d gets on (dest. floor = %d)\n'
                        % (p[0], p[1])).rjust(70))
                    inside[p[1]].append(p[0])
                waiting[f] = []

        log('\t\tt = %-5d floor = %-5d\n' % (t, f))
    
    log('\n[Simulation Complete]\n')

    left_e = [ID for i in inside for ID in i]
    log('\n%d passenger(s) still inside elevator%c' %
        (len(left_e), ':' if len(left_e) > 0 else '.'))
    for ID in left_e: log(' %d' % ID)
    log('\n')

    left_f = [ID for i in waiting for ID in i]
    log('%d passenger(s) waiting on their floor%c' %
        (len(left_f), ':' if len(left_f) > 0 else '.'))
    for p in left_f: log(' %d' % p[0])
    log('\n')

    to_arrive = []
    for i in xrange(1,len(people)+1):
        if wait_lo[i] == -1:
            to_arrive.append(i)
    log('%d passenger(s) yet to arrive on their initial floor%c' %
        (len(to_arrive), ':' if len(to_arrive) > 0 else '.'))
    for ID in to_arrive: log(' %d' % ID)
    log('\n')

    cnt = 0
    tot = 0
    for i in xrange(1,len(people)+1):
        if wait_lo[i] != -1 and wait_hi[i] != -1:
            tot += wait_hi[i] - wait_lo[i] + 1
            cnt += 1
    log('\n' + str(cnt) + ' passengers fully serviced.\n')
    log('Average waiting time for fully serviced passengers: ')
    log('N/A' if cnt == 0 else str(float(tot)/cnt))
    log('\n')

def main():
    parser = argparse.ArgumentParser(
                description='Given info about the elevator and passengers, ' +
                            'as well as a list of commands, simulate the ' +
                            'activities of the elevator and passengers.')
    parser.add_argument('-i', '--input-file', dest='infile',
                        action='store', default='1.in',
                        type=str, metavar='INFILE',
                        help="input file with elevator and passenger info")
    parser.add_argument('-cmd', '--command-file', dest='cmdfile',
                        action='store', default='1.out',
                        type=str, metavar='CMDFILE',
                        help="file containing commands to simulate")
    parser.add_argument('-log', '--log-file', dest='logfile',
                        action='store', default='lift.log',
                        type=str, metavar='LOGFILE',
                        help='after simulation, also dump info to LOGFILE')
    args = parser.parse_args()

    if args.infile is None or args.cmdfile is None:
        print '\tPlease specify an input and command file.'
        print '\tType "simulator.py -h" for further help.'
        return

    try:
        if args.infile is None: raise IOError
        fin = open(args.infile, 'r')
        v = fin.readline().strip().split()
        assert len(v) == 3, 'Incorrect number of values on line 1'
        F = int(v[0]); S = int(v[1]); V = float(v[2])
        N = int(fin.readline().strip())
        assert ((1 <= F <= MAX_F) and (1 <= S <= MAX_S) and (0 < V <= MAX_V)
                and (1 <= N <= MAX_N)), 'Elevator values out of bounds'
        people = []  # list of [ID, T, A, B]
        ID = 1
        for line in fin:
            people.append([ID] + map(int, line.strip().split()))
            assert ((0 <= people[-1][1] <= MAX_T) and
                    (1 <= people[-1][2] <= F) and
                    (1 <= people[-1][3] <= F)), (
                   'Passenger ', ID, ' values out of bounds')
            ID += 1
        assert len(people) == N, 'Number of passengers does not match N'
    except IOError:
        print 'Error opening input file', args.infile
        sys.exit(1)
    except ValueError:
        print 'Error parsing values in input file'
        sys.exit(1)
    except AssertionError, e:
        print 'Error in input file:'
        print '\t', e.message
        sys.exit(1)

    # sort people by the arrival time T
    people = sorted(people, key = lambda x: (x[1]))

    try:
        if args.cmdfile is None: raise IOError
        fcmd = open(args.cmdfile, 'r')
        simulate(fcmd, F, S, V, people)
    except IOError:
        print 'Error opening input file', args.cmdfile
        sys.exit(1)
    except ValueError:
        print 'Error parsing values in commands file'
        sys.exit(1)
    except AssertionError, e:
        print 'Error during simulation:'
        print '\t', e.message
        sys.exit(1)

    global logtext
    logtext = ''.join(logtext)

    print logtext

    try:  # print to log
        if args.logfile is not None:
            flog = open(args.logfile, 'w')
            flog.write(logtext)
            flog.close()
    except IOError:
        print 'Error opening log file', args.logfile
        sys.exit(1)

if __name__ == '__main__':
    main()
