#ifndef NEW_V8_PROGRAM_ERROR_H
#define NEW_V8_PROGRAM_ERROR_H

enum error_code
{
	no_error = 0,
// manager
	get_program_parse_object_error = -3000,
	get_program_import_object_error,
	exist_same_program_name,
//parse
	create_ini_object_error,
	load_describe_file_error,
	
	could_not_find_video_reel_file_error,
	could_not_find_audio_reel_file_error,
	
	get_programid_xml_exception,
	get_programid_unknown_exception,
	get_program_id_error,
	
	get_programInfo_xml_exception,
	get_programInfo_unknown_exception,
	get_programInof_error,
	
	get_programReel_xml_exception,
	get_programReel_unknown_exception,
	could_not_find_reel_error,
	
	get_programHash_xml_exception,
	get_programHash_unknown_exception,
//query
	init_root_path_error,
	get_program_path_error,
	not_find_program_path,
	get_program_list_catch_exception_error,
//import
	program_import_root_error,
	invalid_program_error,
	could_not_find_file_error,
	not_enough_space,
	program_import_error,
	stop_import_error,
	get_program_parse_error,
	get_program_query_error,
	run_thread_manager_error,
//hash
	hash_file_error,
	could_not_find_hash_file_error,
	file_was_destroyed_error,
	no_hash_verify_result_error,
	get_hash_result_error,
//del
    deleteProgram_error
};

#endif
