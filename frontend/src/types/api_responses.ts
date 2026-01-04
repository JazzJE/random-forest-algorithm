// To standardize the status messages which the backend will send for CSV uploads
export type FileStatusType = "success" | "csv_parse_error" | "no_file_inputted";
export interface FileUploadResponse {
  status: FileStatusType;
  message: string;
}

// To standardize backend status messages for model generation
export type ModelStatusType = "success" | "model_failed_to_generate";
export interface ModelGenerateResponse { 
  status : ModelStatusType;
  message : string;
}