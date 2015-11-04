#ifndef DCP_ERROR_CODE_H
#define DCP_ERROR_CODE_H
enum error_code
{
  no_error = 0,

  // kdmOpCtrl
  connect_database_error = -32999,
  exist_dcp_error,
  get_kdm_delete_object_error,
  open_kdm_file_error,
  get_import_object_error,
  get_kdm_verify_object_error,
  create_kdm_file_error = -32000,

  // kdmParser
  invalid_kdm_file = -31999,
  catched_project_element_exception_error,
  get_authenticated_public_info_error,
  get_authenticated_required_extensions_info_error,
  kdmParser_checkFormat_error = -31000,

  // subtitleMaker
  invalid_subtitle_xml_file_error = -13999,
  not_found_content_title_text_error,
  create_project_element_error,
  start_position_beyond_subtitle_number_error,
  start_time_bigger_than_end_time_error,
  start_pos_bigger_than_end_pos_error,
  could_not_find_subtitle_list_error,
  set_sutitle_property_exception_error,
  save_xml_file_error,
  delete_subtitle_catched_exception_error,
  refurbish_spot_number_error = -13000,

  // dcpCopy
  dcpCopy_getDestPath_error = -12999,
  dcpCopy_check_error,
  dcpCopy_stop_error,
  invalid_dcp_error,
  not_enough_space,
  ftp_login_error,
  network_connections_error,
  not_found_file = -12000,

  // dcpQuery 
  dcp_root_path_nesting = -11999,
  invalid_root_path_error,
  dcpQuery_open_error,
  get_cpl_uuid_error,
  get_dcp_list_error,
  dcpQuery_getDcp_error,
  get_dcpParser_object_error,
  exist_same_dcp_name = -11000,

  // dcpManager 
  get_dcpQuery_object_failed = -10999,
  get_dcpCopy_object_failed,
  dcpDelete_init_error,
  cpl_is_using_error,
  get_dcpQuery_object_exception,
  delete_dcp_error,
  get_dcpDelete_object_failed,
  dcpDelete_deleteDcp_error = -10000,

  // updateVersion
  create_ini_object_error,
  get_version_error,
  import_update_package_error,
  get_update_path_error
}; 

#endif
