import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import Tree from 'react-d3-tree';
import './App.css'

const data = {
    name: 'Start',
    children: [
        { name: 'Option 1', children: [{ name: 'End 1' }] },
        { name: 'Option 2', children: [{ name: 'End 2' }] },
    ],
};

function App() {
    const [count, setCount] = useState(0)
    const [backendStatus, setBackendStatus] = useState<string>("")

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
                <button onClick={() => setCount((count) => count + 1)}>
                    count is {count}
                </button>

                <p>
                    Backend status: <strong>{backendStatus}</strong>
                </p>

                <p>
                    Edit <code>src/App.tsx</code> and save to test HMR
                </p>
            </div>

            <p className="read-the-docs">
                Click on the Vite and React logos to learn more
            </p>

            <Tree data={data} orientation="vertical" />
        </>
    )
}

export default App
