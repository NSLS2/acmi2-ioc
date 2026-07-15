"""
baseline_distribution.py
------------------------
Reads Baseline waveforms (Base-Wfm) from the EPICS IOC and plots
histograms with Gaussian fit overlays for TP1, TP2, TP3,
or all three in a 1x3 grid.

Usage:
    python3 baseline_distribution.py                        # all three channels
    python3 baseline_distribution.py --channel tp1          # single channel
    python3 baseline_distribution.py --channel tp2 --bins 80
    python3 baseline_distribution.py --prefix "LN-BI{ACMI2:A}"
"""

import argparse
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
import epics

DEFAULT_PREFIX = "LN-BI{ACMI2:A}"

CHANNELS = {
    "tp1": ("TP1:base-Wfm", "TP1 Baseline"),
    "tp2": ("TP2:base-Wfm", "TP2 Baseline"),
    "tp3": ("TP3:base-Wfm", "TP3 Baseline"),
}

COLORS = {
    "tp1": "steelblue",
    "tp2": "darkorange",
    "tp3": "seagreen",
}

UNITS = "ADC counts"


def fetch_waveform(pv_name: str, timeout: float = 5.0) -> np.ndarray:
    data = epics.caget(pv_name, timeout=timeout)
    if data is None:
        raise RuntimeError(f"Could not read PV: {pv_name!r}  (timeout={timeout}s)")
    data = np.asarray(data, dtype=float)
    last = np.nonzero(data)[0]
    if last.size == 0:
        raise RuntimeError(f"{pv_name}: waveform contains only zeros — no data recorded yet.")
    return data[: last[-1] + 1]


def draw_histogram(ax, data: np.ndarray, bins: int, label: str,
                   color: str, pv_name: str) -> None:
    counts, edges, _ = ax.hist(
        data, bins=bins, color=color, edgecolor="white",
        alpha=0.80, label=f"n = {len(data)}"
    )

    mu, sigma = norm.fit(data)
    x = np.linspace(edges[0], edges[-1], 400)
    bin_width = edges[1] - edges[0]
    pdf_scaled = norm.pdf(x, mu, sigma) * len(data) * bin_width
    ax.plot(x, pdf_scaled, "r-", linewidth=2,
            label=f"Gaussian fit\nμ = {mu:.4f}\nσ = {sigma:.4f}")

    ax.set_xlabel(f"Baseline ({UNITS})", fontsize=10)
    ax.set_ylabel("Counts", fontsize=10)
    ax.set_title(label, fontsize=11)
    ax.legend(fontsize=9)
    ax.grid(True, linestyle="--", alpha=0.4)
    ax.text(0.99, 0.01, pv_name, transform=ax.transAxes,
            ha="right", va="bottom", fontsize=6, color="gray")


def plot_single(prefix: str, key: str, bins: int, timeout: float) -> None:
    suffix, label = CHANNELS[key]
    pv = f"{prefix}{suffix}"
    print(f"Reading {pv} ...")
    data = fetch_waveform(pv, timeout=timeout)
    print(f"  {len(data)} samples  mean={data.mean():.4f}  std={data.std():.4f}")

    fig, ax = plt.subplots(figsize=(8, 5))
    draw_histogram(ax, data, bins, f"{label} Distribution", COLORS[key], pv)
    plt.tight_layout()
    out = f"baseline_dist_{key}.png"
    plt.savefig(out, dpi=150)
    print(f"Saved {out}")


def plot_all(prefix: str, bins: int, timeout: float) -> None:
    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    fig.suptitle("Pulse Baseline Distributions", fontsize=14, fontweight="bold")

    for ax, key in zip(axes.flat, CHANNELS):
        suffix, label = CHANNELS[key]
        pv = f"{prefix}{suffix}"
        print(f"Reading {pv} ...")
        try:
            data = fetch_waveform(pv, timeout=timeout)
            print(f"  {len(data)} samples  mean={data.mean():.4f}  std={data.std():.4f}")
            draw_histogram(ax, data, bins, f"{label} Distribution", COLORS[key], pv)
        except RuntimeError as exc:
            ax.set_title(f"{label} — ERROR")
            ax.text(0.5, 0.5, str(exc), transform=ax.transAxes,
                    ha="center", va="center", fontsize=8, color="red", wrap=True)
            print(f"  WARNING: {exc}")

    plt.tight_layout()
    out = "baseline_dist_all.png"
    plt.savefig(out, dpi=150)
    print(f"Saved {out}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Base-Wfm histogram with Gaussian fit")
    parser.add_argument("--prefix", default=DEFAULT_PREFIX,
                        help=f"IOC PV prefix  (default: {DEFAULT_PREFIX!r})")
    parser.add_argument("--channel", choices=list(CHANNELS) + ["all"], default="all",
                        help="Channel to plot: tp1 tp2 tp3 all  (default: all)")
    parser.add_argument("--bins", type=int, default=60,
                        help="Number of histogram bins (default: 60)")
    parser.add_argument("--timeout", type=float, default=5.0,
                        help="CA get timeout in seconds (default: 5)")
    args = parser.parse_args()

    if args.channel == "all":
        plot_all(args.prefix, bins=args.bins, timeout=args.timeout)
    else:
        plot_single(args.prefix, args.channel, bins=args.bins, timeout=args.timeout)


if __name__ == "__main__":
    main()
