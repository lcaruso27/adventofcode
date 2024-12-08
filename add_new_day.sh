#!/usr/bin/env bash

# author: caruso.lorenzo@ymail.com
# Add code for new AOC day

help()
{
    echo "Add new day executable for AOC."
    echo "Usage: add_new_day.sh -y <year> -d <day_number> -s <summary>"
    echo ""
    echo "-y                    OPTION: AOC year to add. Default is current year."
    echo "-d                    OPTION: Number of the day to add. Default is current day."
    echo "-s                    OPTION: Short summary of day task. Default is title from corresponding 'year' and 'day' puzzle."
    echo ""

    exit 1
}

YEAR=$(date +%Y)
DAYNUM=$(date +%d)
DDAY=$(printf "%d" "$DAYNUM")
SUMMARY=""

while getopts "y:d:s:" option; do
    case ${option} in
        y)
            YEAR="${OPTARG}"
            ;;
        d)
            DAYNUM="$(printf "%02d" ${OPTARG})"
            DDAY="$(printf "%d" ${OPTARG})"
            ;;
        s)
            SUMMARY="${OPTARG}"
            ;;
        *)
            echo "invalid option: ${OPTARG}"
            help
            exit 1
            ;;
    esac
done

shift $((OPTIND - 1))

if [[ "$#" != 0 ]]; then
    help
fi

AOCWEBSITE="https://adventofcode.com"

if [ -z "$SUMMARY" ]; then
    SUMMARY=$(curl -s "${AOCWEBSITE}/${YEAR}/day/${DDAY}" | sed -n '/<h2>/p' | sed -E 's,.*Day.*: (.*) ---.*,\1,')
fi

TOOLDIR=${0%/*}
COOKIE="${TOOLDIR}/.cookie"

mkdir -p "${TOOLDIR}/${YEAR}/day$DAYNUM"

if [ ! -f "${TOOLDIR}/${YEAR}/day$DAYNUM/main.cpp" ]; then
    sed "s,#YYYY,$YEAR,;s,#XX,$DAYNUM,;s,#Subject,${SUMMARY}," "${TOOLDIR}/main.cpp.in" > "${TOOLDIR}/${YEAR}/day$DAYNUM/main.cpp"
    echo -n "" > "${TOOLDIR}/${YEAR}/day$DAYNUM/example.txt"
    curl -b session=$(cat $COOKIE) "${AOCWEBSITE}/${YEAR}/day/${DDAY}/input" > "${TOOLDIR}/${YEAR}/day$DAYNUM/input.txt"
fi

cmakeline="add_executable(${YEAR}_day${DAYNUM}_main day${DAYNUM}/main.cpp)"
if ! grep -Fxq "${cmakeline}" "${TOOLDIR}/${YEAR}/CMakeLists.txt"; then
    echo "${cmakeline}" | tee -a "${TOOLDIR}/${YEAR}/CMakeLists.txt" > /dev/null
fi

exit 0
