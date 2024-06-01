include_guard()

include(GodotJoltExternalLibrary)

set(configurations
	Debug
	Distribution
	EditorDebug
	EditorDistribution
)

set(is_msvc_like $<BOOL:${MSVC}>)
set(is_double_precision $<BOOL:${GDJ_DOUBLE_PRECISION}>)

set(editor_definitions
	DEBUG_ENABLED
	DEBUG_METHODS_ENABLED
	TOOLS_ENABLED
)

gdj_add_external_library(godot-cpp "${configurations}"
	GIT_REPOSITORY https://github.com/godot-jolt/godot-cpp.git
	GIT_COMMIT 0680a37de26a1489f9a75f4ad9b4da5d280d9012
	LANGUAGE CXX
	OUTPUT_NAME godot-cpp
	INCLUDE_DIRECTORIES
		<SOURCE_DIR>/gdextension
		<SOURCE_DIR>/include
		<BINARY_DIR>/gen/include
	COMPILE_DEFINITIONS
		$<${is_msvc_like}:TYPED_METHOD_BIND>
		$<${is_double_precision}:REAL_T_IS_DOUBLE>
	COMPILE_DEFINITIONS_EDITORDEBUG
		${editor_definitions}
	COMPILE_DEFINITIONS_EDITORDEVELOPMENT
		${editor_definitions}
	COMPILE_DEFINITIONS_EDITORDISTRIBUTION
		${editor_definitions}
	CMAKE_CACHE_ARGS
		-DCMAKE_UNITY_BUILD=TRUE
		-DCMAKE_UNITY_BUILD_BATCH_SIZE=32
		-DCMAKE_INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION=${GDJ_INTERPROCEDURAL_OPTIMIZATION}
		-DCMAKE_INTERPROCEDURAL_OPTIMIZATION_EDITORDISTRIBUTION=${GDJ_INTERPROCEDURAL_OPTIMIZATION}
		-DGDE_X86_INSTRUCTION_SET=${GDJ_X86_INSTRUCTION_SET}
		-DGDE_DOUBLE_PRECISION=${GDJ_DOUBLE_PRECISION}
	LIBRARY_CONFIG_DEBUG Debug
	LIBRARY_CONFIG_DEVELOPMENT Distribution
	LIBRARY_CONFIG_DISTRIBUTION Distribution
	LIBRARY_CONFIG_EDITORDEBUG EditorDebug
	LIBRARY_CONFIG_EDITORDEVELOPMENT EditorDistribution
	LIBRARY_CONFIG_EDITORDISTRIBUTION EditorDistribution
)
