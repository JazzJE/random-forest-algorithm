import { useState } from 'react';


interface Props {
    setLoadingStatus : React.Dispatch<React.SetStateAction<boolean>>;
}

function TreeGenerator(
    {
        setLoadingStatus
    } : Props
)
{
    const GenerateTrees = async () => {
        
        setLoadingStatus(true);
        
        try
        {
            
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
            <button onClick={GenerateTrees}>Generate Trees</button>
        </>
    )
}

export default TreeGenerator;