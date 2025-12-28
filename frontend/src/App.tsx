import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import Tree from 'react-d3-tree';
import FileUploader from './FileUploader.tsx'
import './App.css'

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
    const [backendStatus, setBackendStatus] = useState<string>("")

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
            <div>
                <a href="https://vite.dev" target="_blank">
                    <img src={viteLogo} className="logo" alt="Vite logo" />
                </a>
                <a href="https://react.dev" target="_blank">
                    <img src={reactLogo} className="logo react" alt="React logo" />
                </a>
            </div>

            <h1>Vite + React</h1>

            <div className="card">
                <p>
                    Backend status: <strong>{backendStatus}</strong>
                </p>

                <p>
                    Edit <code>src/App.tsx</code> and save to test HMR
                </p>
            </div>

            <Tree data={data} orientation="vertical" />

            <FileUploader
            fileUploadedStatus={fileUploadedStatus}
            setFileUploadedStatus={setFileUploadedStatus}
            headers={headers}
            setHeaders={setHeaders}
            continuousFeatures={continuousFeatures}
            setContinuousFeatures={setContinuousFeatures}
            targetLabel={targetLabel}
            setTargetLabel={setTargetLabel}
            />

            {/* Parent can now react to everything */}
            {fileUploadedStatus.status === "success" && (
                <button>Train Model</button>
            )}
        </>
    )
}

export default App
