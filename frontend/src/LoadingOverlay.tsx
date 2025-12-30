function LoadingOverlay() {
  return (
    <div id="loading-root">
      <div className="loading-overlay">
        <div className="spinner" />
        <p>Processing…</p>
      </div>
    </div>
  );
}

export default LoadingOverlay;