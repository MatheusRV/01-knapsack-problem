#!/bin/bash
{
    clear && clear
    cat <<EOF
 Welcome to 01 Knappack Problem Solver
EOF
    printf "\n ( PRESS KEY '0' FOR GENERATE BINARY )
 ( PRESS KEY '1' FOR GENERATE INSTANCE )
 ( PRESS KEY '2' FOR CPLEX )
 ( PRESS KEY 'Z' FOR CLEAN )\n
 Option: "
    read -r option
    case $option in
    0)
        rm cplex || error=true 
        rm instancia || error=true 
        if [ "${error}" == true ]; then
            error=false
            printf "\n  [ ERROR ] Delete Binary returns a non-zero exit status"
        fi
        make || error=true 
        g++ instancia.cpp -o instancia || error=true 
        if [ "${error}" == true ]; then
            error=false
            printf "\n  [ ERROR ] Error Generating Binary returns a non-zero exit status"
        fi
    ;;
    1)
        printf "\n int Seed (0): "
        read -r seed
        if [ -z "$seed" ] ; then
            seed=0
        fi
        printf " int NrAcademicos (10): "
        read -r NrAcademicos
        if [ -z "$NrAcademicos" ] ; then
            NrAcademicos=10
        fi
        printf " int K (1): "
        read -r K
        if [ -z "$K" ] ; then
            K=1
        fi
        printf " Filename (instancia10): "
        read -r Filename
        if [ -z "$Filename" ] ; then
            Filename="instancia"
        fi

        ./instancia "$seed" "$NrAcademicos" "$K" "$Filename""$NrAcademicos"".txt" || error=true
        wait

        if [ "${error}" == true ]; then
            error=false
            printf "\n  [ ERROR ] Generate Instance returns a non-zero exit status"
        fi
    ;;
    2)
        printf "\n Verbose (0): "
        read -r Verbose
        if [ -z "$Verbose" ] ; then
            Verbose=0
        fi
        printf "\n Filename (instanciaNobel): "
        read -r Filename
        if [ -z "$Filename" ] ; then
            Filename="instanciaNobel"
        fi

        ./cplex 0 "$Filename"".txt" || error=true
        wait

        if [ "${error}" == true ]; then
            error=false
            printf "\n  [ ERROR ] Generate Instance returns a non-zero exit status"
        fi
    ;;
    Z)
        chmod 777 clean.sh
        ./clean.sh || error=true
        if [ "${error}" == true ]; then
            error=false
            printf "\n  [ ERROR ] Cleaning returns a non-zero exit status"
        fi
    ;;
    *) echo "Invalid Option!" ;;
esac

}