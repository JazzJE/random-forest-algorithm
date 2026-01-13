import { useState, useEffect } from 'react'
import Tree from 'react-d3-tree';
import FileUploader from './FileUploader.tsx'
import './App.css'
import RandomForestAlgorithmGenerator from './RandomForestAlgorithmGenerator.tsx';
import LoadingOverlay from './LoadingOverlay.tsx';
import type { FileUploadResponse, ModelGenerateResponse } from './types/api_responses.ts';

const data = {
    name: 'Start',
    children: [
        { name: 'Option 1', children: [{ name: 'End 1' }] },
        { name: 'Option 2', children: [{ name: 'End 2' }] },
    ],
};

function App() {
    // Tell the user if the backend is running
    const [backendStatus, setBackendStatus] = useState<string>("")
    
    // State to disable everything when a process is loading
    const [loadingStatus, setLoadingStatus] = useState<boolean>(false);

    // Collected during file upload process
    const [fileUploadedStatus, setFileUploadedStatus] = useState<FileUploadResponse>({
        status: "no_file_inputted",
        message: "No files uploaded yet"
    });
    const [headers, setHeaders] = useState<string[]>([]);
    const [continuousFeatures, setContinuousFeatures] = useState<Set<string>>(new Set());
    const [targetLabel, setTargetLabel] = useState<string | null>(null);

    // Set during the random forest model generation process
    const [modelGeneratedStatus, setModelGeneratedStatus] = useState<ModelGenerateResponse>({
        status: "no_model_generated",
        message: "No model uploaded yet"
    });

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
            {loadingStatus && <LoadingOverlay />}

            <div className="card">
                <p>Backend status:</p><strong>{backendStatus}</strong>
            </div>

            <Tree data={data} orientation="vertical" />

            {/* Model trainer option, but also ask for how many samples each tree should bootstrap with */}
            {fileUploadedStatus.status === "success" &&
            <RandomForestAlgorithmGenerator
            modelGeneratedStatus={modelGeneratedStatus}
            setModelGeneratedStatus={setModelGeneratedStatus}
            loadingStatus={loadingStatus}
            setLoadingStatus={setLoadingStatus}
            />}

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

            
        </>
    )
}

export default App;
