cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/etl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/tests
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ode_base.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ode_adaptive.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ode_io.cc
    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_backward_euler.cc    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_dopri_54.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_dopri_87.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_embedded.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_erk.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_euler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_gauss_6.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_geng_5.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_grk4a.cc    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_irk.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_linalg.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_lobatto_iiic_6.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_newton.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_radau_iia_5.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_rk.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_rk_4.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_rk_43.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_rkck.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_rkf_32.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_rosenbrock.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_row6a.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_sdirk_43.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_ssp_3.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_trapz.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_vern_65.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_vern_76.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/libode/src/ode_vern_98.cc
)
