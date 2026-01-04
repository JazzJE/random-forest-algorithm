import { useState } from 'react';
import type { ModelGenerateResponse, ModelStatusType } from "./types/api_responses.ts"

interface Props {
    modelGeneratedStatus : ModelGenerateResponse;
    setModelGeneratedStatus : React.Dispatch<React.SetStateAction<ModelGenerateResponse>>;
    loadingStatus : boolean;
    setLoadingStatus : React.Dispatch<React.SetStateAction<boolean>>;
}

function RandomForestAlgorithmGenerator({
        modelGeneratedStatus,
        setModelGeneratedStatus,
        loadingStatus,
        setLoadingStatus
    } : Props)
{
    // Customization options for each tree
    const [numberOfBootstrappedSamplesPerTree, setNumberOfBootstrappedSamplesPerTree] = useState<number>(1);
    const [numberOfTrees, setNumberOfTrees] = useState<number>(1);
    const [maximumDepthLevel, setMaximumDepthLevel] = useState<number>(1);

    const GenerateRandomForestAlgorithm = async () => {
        try
        {
            setLoadingStatus(true);

            const formData = new FormData();
            formData.append("number_of_bootstrapped_samples_per_tree", numberOfBootstrappedSamplesPerTree.toString());
            formData.append("number_of_trees", numberOfTrees.toString());
            formData.append("maximum_depth_level", maximumDepthLevel.toString());

            const res = await fetch("http://localhost:8080/generate_random_forest_algorithm", {
                method: "POST",
                body: formData,
            });

            const data = await res.json();
            
            setModelGeneratedStatus({
                status: data.status as ModelStatusType,
                message: data.message
            });
        }
        catch (err)
        {
            console.error("Tree generation failed: ", err);
        }
        finally
        {
            setLoadingStatus(false);
        }
    }

    return (
        <>
            {/* Input for getting the number of bootstrapped samples each tree should train with*/}
            <label style={{ display: "block" }}>
                Number of bootstrapped samples per tree: 
                <input type="number" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} 
                onChange={(e) => 
                {
                    const v = Math.min(1, Number(e.target.value))
                    setNumberOfBootstrappedSamplesPerTree(v);
                }}/>
            </label>

            {/* Input for getting the number of trees in the algorithm desired */}
            <label style={{ display: "block" }}>
                Number of trees:
                <input type="number" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"}
                onChange={(e) =>  {
                    const v = Math.max(1, Math.min(255, Number(e.target.value))); // Enforce max and min values of the input
                    setNumberOfTrees(v)
                }} />
            </label>

            {/* Input for getting the maximum depth level for each tree before terminating */}
            <label style={{ display: "block" }}>
                Tree depth level:
                <input type="number" min="1" max="255" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"}
                onChange={(e) =>  {
                    const v = Math.max(1, Math.min(255, Number(e.target.value))); // Enforce max and min values of the input
                    setMaximumDepthLevel(v)
                }} />
            </label>

            <button onClick={GenerateRandomForestAlgorithm}>Generate Random Forest Algorithm</button>
        </>
    )
}

export default RandomForestAlgorithmGenerator;