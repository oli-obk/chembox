use std::ops::Index;

use crate::pipe::Pipe;
use spanned::Spanned;

pub struct Grid {
    data: Vec<Vec<Element>>,
}

impl Index<(u32, u32)> for Grid {
    type Output = Element;

    fn index(&self, (x, y): (u32, u32)) -> &Self::Output {
        const DEFAULT: &Element = &Element::Pipe(Pipe::EMPTY);
        self.get((x, y)).unwrap_or(DEFAULT)
    }
}

impl Grid {
    pub fn parse<'a>(lines: impl Iterator<Item = Spanned<&'a str>>) -> Option<Self> {
        Some(Grid {
            data: lines
                .map(|line| line.chars().map(Element::parse).collect())
                .collect(),
        })
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
    pub fn index_and_rotation(&self) -> (u8, u8) {
        match self {
            Element::Pipe(p) => p.index_and_rotation(),
            Element::Pump => (18, 0),
        }
    }
}
