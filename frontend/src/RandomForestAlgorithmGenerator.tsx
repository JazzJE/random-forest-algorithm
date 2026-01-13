import { useRef } from 'react';
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

    // For model generation
    const numberOfBootstrappedSamplesInputRef = useRef<HTMLInputElement>(null);
    const numberOfTreesInputRef = useRef<HTMLInputElement>(null);
    // For tree generation
    const maxDepthLevelInputRef = useRef<HTMLInputElement>(null);
    const minSampleSplitInputRef = useRef<HTMLInputElement>(null);
    const maxLeafNodesInputRef = useRef<HTMLInputElement>(null);
    const minEntropyInputRef = useRef<HTMLInputElement>(null);

    const GenerateRandomForestAlgorithm = async () => {

        // Check if the number of bootstrapped samples per tree if it is empty, less than 1 or is an invalid input
        const bootstrapSamplesPerTree = Number(numberOfBootstrappedSamplesInputRef.current?.value);
        if (
            Number.isNaN(bootstrapSamplesPerTree) || 
            !Number.isInteger(bootstrapSamplesPerTree) ||
            bootstrapSamplesPerTree < 1
        )
        {
            alert("[ERROR] Please ensure you enter an integer greater than or equal to 1 for number of bootstrapped samples per tree.");
            return;
        }

        // Check if the number of trees inputted is empty, less than 1 or is an invalid input
        const numberOfTrees = Number(numberOfTreesInputRef.current?.value);
        if (
            Number.isNaN(numberOfTrees) || 
            !Number.isInteger(numberOfTrees) || 
            numberOfTrees < 1
        )
        {
            alert("[ERROR] Please ensure you enter an integer greater than or equal to 1 for number of trees.");
            return;
        }

        // Check if the max depth level for each tree is empty, less than 1 or is an invalid input
        const maxDepthLevel = Number(maxDepthLevelInputRef.current?.value);
        if (
            Number.isNaN(maxDepthLevel) || 
            !Number.isInteger(maxDepthLevel) || 
            maxDepthLevel < 1
        )
        {
            alert("[ERROR] Please ensure you enter an integer greater than or equal to 1 for the maximum depth level.");
            return;
        }

        // Check if the minimum number of samples to split is empty, less than 1, or is an invalid input
        const minSampleSplit = Number(minSampleSplitInputRef.current?.value);
        if (
            Number.isNaN(minSampleSplit) || 
            !Number.isInteger(minSampleSplit) || 
            minSampleSplit < 1
        )
        {
            alert("[ERROR] Please ensure you enter an integer greater than or equal to 1 for the minimum sample split.");
            return;
        }

        // Check if the minimum number of samples to split is empty, less than 1, or is an invalid input
        const maxLeafNodes = Number(maxLeafNodesInputRef.current?.value);
        if (
            Number.isNaN(maxLeafNodes) || 
            !Number.isInteger(maxLeafNodes) || 
            maxLeafNodes < 1
        )
        {
            alert("[ERROR] Please ensure you enter an integer greater than or equal to 1 for the max number of leaf nodes.");
            return;
        }

        // Check if the minimum number of samples to split is empty, less than 1, or is an invalid input
        const minEntropy = Number(minEntropyInputRef.current?.value);
        if (Number.isNaN(minEntropy) || minEntropy > 1 || minEntropy < 0)
        {
            alert("[ERROR] Please ensure you enter a double value greater than 0 but less than 1 for the minimum entropy a sample set can have.");
            return;
        }

        try
        {
            setLoadingStatus(true);

            const formData = new FormData();
            
            formData.append("number_of_bootstrapped_samples_per_tree", bootstrapSamplesPerTree.toString());
            formData.append("number_of_trees", numberOfTrees.toString());

            formData.append("maximum_depth_level", maxDepthLevel.toString());
            formData.append("min_sample_split", minSampleSplit.toString());
            formData.append("max_leaf_nodes", maxLeafNodes.toString());
            formData.append("min_entropy", minEntropy.toString());

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
            {/* Inputs for the model */}
            <h2>Model Generation Configs</h2>
            <label style={{ display: "block" }}>
                Number of bootstrapped samples per tree: 
                <input ref={numberOfBootstrappedSamplesInputRef} type="number" step="1" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} />
            </label>
            <label style={{ display: "block" }}>
                Number of trees:
                <input ref={numberOfTreesInputRef} type="number" step="1" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} />
            </label>

            {/* Inputs for the trees */}
            <h2>Terminating Configs for Trees</h2>
            <label style={{ display: "block" }}>
                Tree depth level:
                <input ref={maxDepthLevelInputRef} type="number" step="1" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} />
            </label>
            <label style={{ display: "block" }}>
                Minimum samples for split:
                <input ref={minSampleSplitInputRef} type="number" step="1" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} />
            </label>
            <label style={{ display: "block" }}>
                Maximum leaf nodes:
                <input ref={maxLeafNodesInputRef} type="number" step="1" min="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} />
            </label>
            <label style={{ display: "block" }}>
                Minimum entropy (double value):
                <input ref={minEntropyInputRef} type="number" min="0" max="1" disabled={loadingStatus === true || modelGeneratedStatus.status === "success"} />
            </label>

            <button disabled={loadingStatus || modelGeneratedStatus.status === "success"} onClick={GenerateRandomForestAlgorithm}>Generate Random Forest Algorithm</button>
        </>
    )
}

export default RandomForestAlgorithmGenerator;