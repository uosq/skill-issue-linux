# Engine Library

## Methods

### > IsInGame( )
returns **bool**

### > IsConnected()
returns **bool**

### > IsTakingScreenshot()
returns **bool**

### > IsGameUIVisible()
returns **bool**

### > IsConsoleVisible( )
returns **bool**

### > SetViewAngles( angle: **Vector3** )

### > GetViewAngles( )
returns **Vector3**

### > WorldToScreen( position: **Vector3**  )
returns **Vector3?**

### > GetNetChannel( )
returns **NetChannel**

### > Trace( start: **Vector3**, end: **Vector3**, mask: **int**, shouldHitFunction(entity: **Entity**, contentsMask: **int**) )
returns **Trace**

### > TraceHull( start: **Vector3**, end: **Vector3**, mins: **Vector3**, maxs: **Vecor3**, mask: **int**, shouldHitFunction(entity: **Entity**, contentsMask: **int**) )
returns **Trace**

### > GetPointContent( position: **Vector3** )
returns **int**