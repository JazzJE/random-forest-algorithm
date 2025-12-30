import { useState, useEffect } from 'react'
import Tree from 'react-d3-tree';
import FileUploader from './FileUploader.tsx'
import './App.css'
import TreeGenerator from './TreeGenerator.tsx';
import LoadingOverlay from './LoadingOverlay.tsx'

const data = {
    name: 'Start',
    children: [
        { name: 'Option 1', children: [{ name: 'End 1' }] },
        { name: 'Option 2', children: [{ name: 'End 2' }] },
    ],
};

// To standardize the status messages which the backend will send
type StatusType = "success" | "csv_parse_error" | "no_file_inputted";
interface fileResponse {
  status: StatusType;
  message: string;
}

function App() {
    // Tell the user if the backend is running
    const [backendStatus, setBackendStatus] = useState<string>("")
    
    // State to disable everything when a process is loading
    const [loadingStatus, setLoadingStatus] = useState<boolean>(false);

    // Collected during file upload process
    const [fileUploadedStatus, setFileUploadedStatus] = useState<fileResponse>({
        status: "no_file_inputted",
        message: "No files uploaded yet"
    });
    const [headers, setHeaders] = useState<string[]>([]);
    const [continuousFeatures, setContinuousFeatures] = useState<Set<string>>(new Set());
    const [targetLabel, setTargetLabel] = useState<string | null>(null);

    // Fetch backend on mount
    useEffect(() => {
        fetch("http://localhost:8080/status")
            .then(res => res.text())
            .then(text => {
                console.log("Backend says:", text)
                setBackendStatus(text)
            })
            .catch(err => {
                console.error("Error contacting backend:", err)
                setBackendStatus("Error: Could not connect to backend.")
            })
    }, [])

    return (
        <>
            <div className="card">
                <p>
                    Backend status: <strong>{backendStatus}</strong>
                </p>
            </div>

            <Tree data={data} orientation="vertical" />
            
            {/* Model trainer option */}
            {fileUploadedStatus.status === "success" && (<TreeGenerator/>)}

            <FileUploader
            fileUploadedStatus={fileUploadedStatus}
            setFileUploadedStatus={setFileUploadedStatus}
            headers={headers}
            setHeaders={setHeaders}
            continuousFeatures={continuousFeatures}
            setContinuousFeatures={setContinuousFeatures}
            targetLabel={targetLabel}
            setTargetLabel={setTargetLabel}
            loadingStatus={loadingStatus}
            setLoadingStatus={setLoadingStatus}
            />

            {loadingStatus && <LoadingOverlay />}
        </>
    )
}

export default App;
