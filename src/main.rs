use bevy::prelude::*;
use bevy_ecs_tilemap::prelude::*;
use bevy_pancam::{PanCam, PanCamPlugin};
use grid::Grid;
use spanned::Spanned;

mod grid;
mod pipe;

fn startup(mut commands: Commands, asset_server: Res<AssetServer>, windows: Query<&Window>) {
    commands.spawn(Camera2dBundle::default()).insert(PanCam {
        grab_buttons: vec![MouseButton::Middle], // which buttons should drag the camera
        enabled: true,        // when false, controls are disabled. See toggle example.
        zoom_to_cursor: true, // whether to zoom towards the mouse or the center of the screen
        min_scale: 1.,        // prevent the camera from zooming too far in
        ..Default::default()
    });

    let texture_handle = pipe::PIPES
        .iter()
        .map(|&path| asset_server.load(path))
        .collect::<Vec<_>>();

    let grid = Spanned::read_from_file("data/test.grid").unwrap();
    let grid = Grid::parse(grid.lines()).unwrap();

    let map_size = TilemapSize {
        x: grid.width(),
        y: grid.height(),
    };

    // Create a tilemap entity a little early.
    // We want this entity early because we need to tell each tile which tilemap entity
    // it is associated with. This is done with the TilemapId component on each tile.
    // Eventually, we will insert the `TilemapBundle` bundle on the entity, which
    // will contain various necessary components, such as `TileStorage`.
    let tilemap_entity = commands.spawn_empty().id();

    // To begin creating the map we will need a `TileStorage` component.
    // This component is a grid of tile entities and is used to help keep track of individual
    // tiles in the world. If you have multiple layers of tiles you would have a tilemap entity
    // per layer, each with their own `TileStorage` component.
    let mut tile_storage = TileStorage::empty(map_size);

    // Spawn the elements of the tilemap.
    // Alternatively, you can use helpers::filling::fill_tilemap.
    for x in 0..map_size.x {
        for y in 0..map_size.y {
            let tile_pos = TilePos { x, y };
            let (index, rotation) = grid[(x, y)].index_and_rotation();
            let flip = match rotation {
                0 => TileFlip::default(),
                1 => TileFlip {
                    x: true,
                    y: false,
                    d: true,
                },
                2 => TileFlip {
                    x: true,
                    y: true,
                    d: false,
                },
                3 => TileFlip {
                    x: false,
                    y: true,
                    d: true,
                },
                _ => unreachable!(),
            };
            let tile_entity = commands
                .spawn(TileBundle {
                    position: tile_pos,
                    tilemap_id: TilemapId(tilemap_entity),
                    texture_index: TileTextureIndex(index.into()),
                    flip,
                    ..Default::default()
                })
                .id();
            tile_storage.set(&tile_pos, tile_entity);
        }
    }

    let tile_size = TilemapTileSize { x: 500.0, y: 500.0 };
    let map_type = TilemapType::Square;

    commands.entity(tilemap_entity).insert(TilemapBundle {
        grid_size: tile_size.into(),
        map_type,
        size: map_size,
        storage: tile_storage,
        texture: TilemapTexture::Vector(texture_handle),
        tile_size,
        transform: Transform::from_scale((1.0 / 16.0, 1.0 / 16.0, 1.0).into()).with_translation(
            (
                -windows.single().width() / 2.0 + 50.0,
                -windows.single().height() / 2.0 + 50.0,
                0.0,
            )
                .into(),
        ),
        ..Default::default()
    });
}

fn main() {
    App::new()
        .add_plugins(DefaultPlugins.set(ImagePlugin::default_nearest()))
        .add_plugins(PanCamPlugin::default())
        .add_plugins(TilemapPlugin)
        .add_systems(Startup, startup)
        .add_systems(Update, camera)
        .run();
}

fn camera(
    camera_query: Query<(&Camera, &GlobalTransform)>,
    windows: Query<&Window>,
    mut gizmos: Gizmos,
) {
    let (camera, camera_transform) = camera_query.single();

    let Some(cursor_position) = windows.single().cursor_position() else {
        return;
    };

    // Calculate a world position based on the cursor's position.
    let Some(point) = camera.viewport_to_world_2d(camera_transform, cursor_position) else {
        return;
    };

    gizmos.circle_2d(point, 10., Color::WHITE);
}
