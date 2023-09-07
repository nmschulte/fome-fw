#!/usr/bin/env bash

set -e

PROJECT_BOARD="${1}"; [ "$#" -ge 1 ] && shift

# --

log_message() {
    message="${1}"
    echo -e "\033[31;1;4m\n# ${message}\n\033[0m"
}

get_board_params() {
    board="${1}"

    target=
    folder=
    ini=

    case "${board}" in
        *)
            target="${board}"
            folder="config/boards/${board}"
            ini="fome_${board}.ini"
            ;;&
        alphax-*|harley81|hellen*)
            folder="config/boards/hellen/${board}"
            ;;&
        hellen88bmw*)
            ini="fome_hellen88bmw.ini"
            ;;
        hellenNA6)
            folder="hellen/hellen64_miataNA6_94"
            ini="fome_hellenNA6.ini"
            ;;
        hellen128)
            ini="fome_hellen128mercedes.ini"
            ;;
        kinetis)
            ini="fome_kin.ini"
            ;;
        mre_*)
            folder="config/boards/microrusefi"
            ;;&
        mre_f4*)
            ini="fome_mre_f4.ini"
            ;;
        mre_f7*)
            # TODO(nms): console-settings: firmware/config/boards/nucleo_f767/rusefi_console_properties.xml
            ini="fome_mre_f7.ini"
            ;;
        prometheus_*)
            folder="config/boards/prometheus"
            ;;
        proteus_*)
            folder="config/boards/proteus"
            ;;&
        proteus_f4_*)
            ini="fome_proteus_f4.ini"
            ;;
        proteus_f7_*|proteus_legacy)
            ini="fome_proteus_f7.ini"
            ;;
        stm32*_nucleo)
            ;;&
        stm32f429_nucleo)
            folder="config/boards/nucleo_f429"
            ;;
        stm32f767_nucleo)
            # TODO(nms): console-settings: firmware/config/boards/nucleo_f767/rusefi_console_properties.xml
            folder="config/boards/nucleo_f767"
            ;;
        stm32h743_nucleo)
            folder="config/boards/nucleo_h743"
            ;;
        subaru_eg33_f7)
            folder="config/boards/subaru_eg33"
            ;;
    esac

    echo "${target} ${folder} ${ini}"
}

# --

board_params=($(get_board_params "${PROJECT_BOARD}"))
params_target="${board_params[0]}"
params_folder="${board_params[1]}"
params_ini="${board_params[2]}"

log_message "Generate Live Documentation"
./gen_live_documentation.sh

log_message "Generate Enum Strings"
./gen_enum_to_string.sh

log_message "Generate Default Config"
./gen_config_default.sh

log_message "Generate \$PROJECT_BOARD=${PROJECT_BOARD} Config"
case "${PROJECT_BOARD}" in
    kinetis)
        ./config/boards/kinetis/config/gen_kinetis_config.sh
        ;;
    cypress)
        ./config/boards/cypress/config/gen_cypress_config.sh
        ;;
    subaru_eg33_f7)
        ./config/boards/subaru_eg33/config/gen_subaru_config.sh
        ;;
    *)
        ./gen_config_board.sh "${params_folder}" "${params_target}" "${params_ini}"
        ;;
esac

log_message "Build Console"
pushd ..; ./misc/jenkins/build_java_console.sh; popd

log_message "Build Simulator"
pushd ..; ./misc/jenkins/build_simulator.sh; popd
#OS="Windows_NT" ../misc/jenkins/build_simulator.sh

log_message "Build Firmware"
make clean; pushd ${params_folder}; ./compile_${params_target}.sh; popd
#../misc/jenkins/compile_other_versions/compile.sh "${params_folder}" "${params_target}"
#./clean_compile_two_versions.sh "PROJECT_BOARD=${PROJECT_BOARD}" "$@"
#cd ..; OS="Windows_NT" misc/jenkins/functional_test_and_build_bundle/build_current_bundle.sh
#cd ..; misc/jenkins/functional_test_and_build_bundle/build_current_bundle.sh
