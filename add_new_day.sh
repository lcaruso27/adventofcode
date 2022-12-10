#!/usr/bin/env bash

# author: caruso.lorenzo@ymail.com
# date: 12/04/2022
# Add code for new AOC day

help()
{
    echo "Add new day executable for AOC."
    echo "Usage: add_new_day.sh <year> <day_number> <description>"
    echo ""
    echo "year                  INPUT: AOC year to add."
    echo "day_number            INPUT: Number of the day to add."
    echo "description           INPUT: Short description of day task."
    echo ""

    exit 1
}

if [[ "$#" != 3 ]]; then
    help
fi

YEAR=$1
DAYNUM=$(printf "%02d" $2)
DDAY=$(printf "%d" $2)
DESC=$3
TOOLDIR=${0%/*}
COOKIE="${TOOLDIR}/${YEAR}/.cookie"

sed "s,#YYYY,$YEAR,;s,#XX,$DAYNUM,;s,#Subject,${DESC}," "${TOOLDIR}/main.cpp.in" > "${TOOLDIR}/${YEAR}/day$DAYNUM/main.cpp"

echo -n "" > "${TOOLDIR}/${YEAR}/day$DAYNUM/example.txt" 
curl -b session=$(cat $COOKIE) "https://adventofcode.com/${YEAR}/day/${DDAY}/input" > "${TOOLDIR}/${YEAR}/day$DAYNUM/input.txt"

echo "add_executable(day${DAYNUM}_main day${DAYNUM}/main.cpp)" | tee -a "${TOOLDIR}/${YEAR}/CMakeLists.txt" > /dev/null

exit 0
