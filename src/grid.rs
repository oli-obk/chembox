use std::ops::{Index, IndexMut};

use crate::pipe::{Directions, Pipe, Rotation};
use spanned::Spanned;

#[derive(Debug)]
pub struct Grid {
    data: Vec<Vec<Element>>,
}

impl IndexMut<(u32, u32)> for Grid {
    fn index_mut(&mut self, (x, y): (u32, u32)) -> &mut Self::Output {
        if y as usize >= self.data.len() {
            self.data.resize_with(y as usize + 1, Vec::new);
        }
        let row = &mut self.data[y as usize];

        if x as usize >= row.len() {
            row.resize_with(x as usize + 1, || Element::Pipe(Pipe::EMPTY));
        }
        &mut row[x as usize]
    }
}

impl Index<(u32, u32)> for Grid {
    type Output = Element;

    fn index(&self, (x, y): (u32, u32)) -> &Self::Output {
        const DEFAULT: &Element = &Element::Pipe(Pipe::EMPTY);
        self.get((x, y)).unwrap_or(DEFAULT)
    }
}

impl Grid {
    fn get(&self, (x, y): (u32, u32)) -> Option<&Element> {
        self.data.get(y as usize)?.get(x as usize)
    }

    pub fn parse<'a>(lines: impl Iterator<Item = Spanned<&'a str>>) -> Self {
        let mut grid = Grid {
            data: lines
                .map(|line| line.chars().map(Element::parse).collect())
                .collect(),
        };
        // Set connections that were not encoded in the symbols.
        for x in 0..grid.width() {
            for y in 0..grid.height() {
                let mut dirs = grid[(x, y)].connections();
                {
                    if let Some(y2) = y.checked_sub(1) {
                        if grid[(x, y2)].connections().down {
                            dirs.up = true;
                        }
                    }
                    if let Some(y2) = y.checked_add(1) {
                        if grid[(x, y2)].connections().up {
                            dirs.down = true;
                        }
                    }
                    if let Some(x2) = x.checked_sub(1) {
                        if grid[(x2, y)].connections().right {
                            dirs.left = true;
                        }
                    }
                    if let Some(x2) = x.checked_add(1) {
                        if grid[(x2, y)].connections().left {
                            dirs.right = true;
                        }
                    }
                }
                let Element::Pipe(p) = &mut grid[(x, y)] else {
                    unreachable!()
                };
                p.set_dirs(dirs);
            }
        }
        grid
    }

    pub(crate) fn height(&self) -> u32 {
        self.data.len().try_into().unwrap()
    }

    pub(crate) fn width(&self) -> u32 {
        self.data
            .iter()
            .map(|l| l.len())
            .max()
            .unwrap_or_default()
            .try_into()
            .unwrap()
    }
}

#[derive(Debug)]
pub enum Element {
    Pipe(Pipe),
    Pump,
}

impl Default for Element {
    fn default() -> Self {
        Self::Pipe(Default::default())
    }
}

impl Element {
    pub fn parse(c: Spanned<char>) -> Self {
        if let Some(pipe) = Pipe::parse(&c) {
            return Self::Pipe(pipe);
        }
        match c {
            _ => panic!("{}: unknown char", c.span),
        }
    }
    pub fn index_and_rotation(&self) -> (u8, Rotation) {
        match self {
            Element::Pipe(p) => {
                let (tile, drain, rot) = p.tile_and_rotation();
                (tile.index(drain), rot)
            }
            Element::Pump => (18, Rotation::Zero),
        }
    }

    fn connections(&self) -> Directions<bool> {
        match self {
            Element::Pipe(p) => p.parts().1,
            Element::Pump => todo!(),
        }
    }
}
