import { useState, useRef } from "react";
import type { FileUploadResponse, FileStatusType } from "./types/api_responses.ts"

interface Props {
  fileUploadedStatus: FileUploadResponse;
  setFileUploadedStatus: React.Dispatch<React.SetStateAction<FileUploadResponse>>;
  headers: string[]
  setHeaders: React.Dispatch<React.SetStateAction<string[]>>;
  continuousFeatures : Set<string>;
  setContinuousFeatures: React.Dispatch<React.SetStateAction<Set<string>>>;
  targetLabel : string | null;
  setTargetLabel: React.Dispatch<React.SetStateAction<string | null>>;
  loadingStatus : boolean;
  setLoadingStatus: React.Dispatch<React.SetStateAction<boolean>>;
}

function FileUploader({
    fileUploadedStatus,
    setFileUploadedStatus,
    headers,
    setHeaders,
    continuousFeatures,
    setContinuousFeatures,
    targetLabel,
    setTargetLabel,
    loadingStatus,
    setLoadingStatus,
  }: Props) {

  const [selectedFile, setSelectedFile] = useState<File | null>(null);

  const fileInputRef = useRef<HTMLInputElement>(null);

  // Validation for checking if the uploaded file is a CSV file
  const isLikelyCSV = (file: File) => {
    const validMimeTypes = [
      "text/csv",
      "application/vnd.ms-excel"
    ];

    const hasCsvExtension = file.name.toLowerCase().endsWith(".csv");

    return validMimeTypes.includes(file.type) || hasCsvExtension;
  };

  // Get the headers of the file without parsing anything else; backend will parse
  const parseHeaders = async (file: File) => {
    const text = await file.text();          // Read entire file as string
    const firstLine = text.split(/\r?\n/)[0]; // Get the first line
    const headers = firstLine.split(",");

    return headers;
  };

  const onFileChange = async (e: React.ChangeEvent<HTMLInputElement>) => {
    // Verify the file is a CSV, and if it isn't clear the preexistent file
    const chosenFile = e.target.files?.[0] ?? null;
    if (!chosenFile || !isLikelyCSV(chosenFile)) {
      alert("[ERROR] Please upload a CSV file");
      setFileUploadedStatus(
        { status: "no_file_inputted", message: "No file is currently detected"}
      );
      setSelectedFile(null);
      return;
    }

    // Update the states for each status
    const newHeaders = await parseHeaders(chosenFile);
    setHeaders(newHeaders);
    setContinuousFeatures(new Set());
    setTargetLabel(null);
    setSelectedFile(chosenFile);
  };

  const onFileUpload = async () => {
    // If there is no file uploaded, then return and alert the user
    if (!selectedFile) {
      alert("[ERROR] No file selected!");
      return;
    }

    // If no target label was selected, then return and alert the user
    if (!targetLabel) {
      alert("[ERROR] No target label was selected!");
      return;
    }

    try {
      setLoadingStatus(true);

      // Append form data
      const formData = new FormData();
      formData.append("file", selectedFile); // MUST match server key
      formData.append("headers", JSON.stringify([...headers]));
      formData.append("continuous_features", JSON.stringify([...continuousFeatures]));
      formData.append("target_label", targetLabel);

      const res = await fetch("http://localhost:8080/upload_dataset", {
        method: "POST",
        body: formData,
      });

      if (!res.ok) {
        throw new Error(`${res.status}`);
      }

      const data = await res.json();

      setFileUploadedStatus({
        status: data.status as FileStatusType,
        message: data.message
      });

    } catch (err) {
      console.error("Upload failed:", err);
    }
    finally {
      setLoadingStatus(false);
    }
  };

  return (
    <div>
      <div>
        <input type="file" ref={fileInputRef} accept=".csv,text/csv" onChange={onFileChange} disabled={loadingStatus === true} />
        <button onClick={onFileUpload} disabled={loadingStatus === true}>Upload to Server</button>
        <p>File Uploaded Status: <strong>{fileUploadedStatus.message}</strong></p>
      </div>
      {selectedFile && (
        <div>

          {/* Metadata */}
          <p><strong>Name</strong>: {selectedFile.name}</p>
          <p><strong>Size</strong>: {selectedFile.size} bytes</p>

          {/* Continuous feature elements chooser */}
          <p>Select the feature columns that should be treated as <strong><em>continuous</em></strong>:</p>
          <div id="continuous-feature-checklist">
          {headers.map((header) => (
            <label key={header} style={{ display: "block" }}>
              <input
                type="checkbox"
                disabled={targetLabel === header || fileUploadedStatus.status === "success" || loadingStatus === true}
                checked={continuousFeatures.has(header)}
                onChange={(e) => {
                  setContinuousFeatures(prev => {
                    const next = new Set(prev);
                    if (e.target.checked) {
                      next.add(header);
                    } else {
                      next.delete(header);
                    }
                    return next;
                  });
                }}
              />
              {header}
            </label>
          ))}
          </div>

          {/* Target label chooser */}
          <p>Select <strong><em>one</em></strong> of your feature columns to be your <strong><em>target labels</em></strong>:</p>
          <div>
            {headers.map((header) => (
              <label key={header} style={{ display: "block" }}>
                <input
                  type="radio"
                  disabled={continuousFeatures.has(header) || fileUploadedStatus.status === "success" || loadingStatus === true}
                  name="targetLabel"
                  value={header}
                  checked={targetLabel === header}
                  onChange={() => setTargetLabel(header)}
                />
                {header}
              </label>
            ))}
          </div>

        </div>
      )}
    </div>
  );
}

export default FileUploader;  