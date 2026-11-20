# __LAN8742 Part Drivers__

![tag](https://img.shields.io/badge/tag-v2.0.0-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](Release_Notes.html)

## Overview

The LAN8742 part drivers provides APIs to drive external Ethernet PHY LAN8742.

## Description and Usage

### Peripherals initialization:

Part LAN8742 assumes that the initialization of all needed peripherals (GPIO, ETH) is done by the main application.

## How to use it?

To use the LAN8742 part API, the application should define the `lan8742_object_t` object,
then call `lan8742_init()`.

Once this is successfully done, all the other part APIs can be used freely.
