add_executable(test_circle_moon test_circle_moon.cpp)
target_link_libraries(test_circle_moon OrbitDyn)

add_executable(test_DE405_DE421 test_DE405_DE421.cpp)
target_link_libraries(test_DE405_DE421 OrbitDyn)

add_executable(test_earth_lunar_transfer test_earth_lunar_transfer.cpp)
target_link_libraries(test_earth_lunar_transfer OrbitDyn)

add_executable(test_facility test_facility.cpp)
target_link_libraries(test_facility OrbitDyn)

add_executable(checkmemoryleak checkmemoryleak.cpp)
target_link_libraries(checkmemoryleak OrbitDyn)

add_executable(test_mean_inst test_mean_inst.cpp)
target_link_libraries(test_mean_inst OrbitDyn)

add_executable(test_orbit_propagator test_orbit_propagator.cpp)
target_link_libraries(test_orbit_propagator OrbitDyn)

add_executable(test_orbit_maneuver test_orbit_maneuver.cpp)
target_link_libraries(test_orbit_maneuver OrbitDyn)

add_executable(test_rapidsattlite test_rapidsattlite.cpp)
target_link_libraries(test_rapidsattlite OrbitDyn)

add_executable(test_RIC test_RIC.cpp)
target_link_libraries(test_RIC OrbitDyn)

add_executable(test_sg test_sg.cpp)
target_link_libraries(test_sg OrbitDyn)

add_executable(test_sofa_Gst test_sofa_Gst.cpp)
target_link_libraries(test_sofa_Gst OrbitDyn sofa)

add_executable(testBackward testBackward.cpp)
target_link_libraries(testBackward OrbitDyn)

add_executable(test_access test_access.cpp)
target_link_libraries(test_access OrbitDyn)

add_executable(testjson testjson.cpp)
target_link_libraries(testjson OrbitDyn)
