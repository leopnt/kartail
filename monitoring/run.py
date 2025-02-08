import numpy as np
import polars as pl
import rerun as rr
import rerun.blueprint as rrb
from datetime import timedelta


def log_gps_trace(df: pl.DataFrame):
    rr.log(
        "gps/trace",
        rr.GeoLineStrings(
            lat_lon=df.select("latitude", "longitude").to_numpy(),
            radii=rr.Radius.ui_points(2.0),
            colors=[[80, 80, 80] for _ in range(len(df))],
        ),
        static=True,
    )


def log_gps_location(df: pl.DataFrame):
    rr.send_columns(
        "gps/location",
        indexes=[
            rr.TimeNanosColumn(
                "datetime",
                df.select("datetime").to_series().to_numpy(),
            )
        ],
        columns=rr.GeoPoints.columns(
            positions=df.select("latitude", "longitude").to_numpy(),
            radii=[rr.Radius.ui_points(6.0) for _ in range(len(df_upsampled))],
            colors=[[255, 0, 0] for _ in range(len(df_upsampled))],
        ),
    )


def log_gps_altitude(df: pl.DataFrame):
    rr.send_columns(
        "gps/altitude",
        indexes=[
            rr.TimeNanosColumn(
                "datetime",
                df.select("datetime").to_series().to_numpy(),
            )
        ],
        columns=rr.Scalar.columns(
            scalar=df.select("altitude").to_numpy(),
        ),
    )


if __name__ == "__main__":
    rr.init("Kartail monitoring", spawn=True)

    df = pl.read_csv(
        "/Users/leopnt/Downloads/0bbce73/gps/2025-02-05T074234Z.csv",
        try_parse_dates=True,
    )

    fps_target = 10
    target_interval = 1 / fps_target

    df_upsampled = df.upsample(
        time_column="datetime", every=timedelta(seconds=target_interval)
    ).interpolate()

    log_gps_trace(df_upsampled)
    log_gps_location(df_upsampled)
    log_gps_altitude(df_upsampled)

    blueprint = rrb.Blueprint(
        rrb.Vertical(
            rrb.MapView(
                origin="gps",
                name="MapView",
            ),
            rrb.TimeSeriesView(origin="gps/altitude", name="GPS altitude (meters)"),
            row_shares=[4, 1],
        ),
        collapse_panels=True,
    )

    rr.send_blueprint(blueprint)
