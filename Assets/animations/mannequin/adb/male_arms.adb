<AnimDB FragDef="animations/mannequin/adb/male_arms_fragmentIds.xml" TagDef="animations/mannequin/adb/male_arms_tags.xml">
	<FragmentList>
		<idle>
			<Fragment BlendOutDuration="0.2" Tags="Pistol">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="pistol_idle" flags="Loop"/>
				</AnimLayer>
			</Fragment>
			<Fragment BlendOutDuration="0.2" Tags="AmericanShotgun">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="shotgun_idle" flags="Loop"/>
				</AnimLayer>
			</Fragment>
		</idle>
		<walk>
			<Fragment BlendOutDuration="0.2" Tags="Pistol">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="pistol_walk" flags="Loop"/>
				</AnimLayer>
			</Fragment>
			<Fragment BlendOutDuration="0.2" Tags="AmericanShotgun">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="shotgun_walk" flags="Loop"/>
				</AnimLayer>
			</Fragment>
		</walk>
		<shoot>
			<Fragment BlendOutDuration="0.2" Tags="Pistol">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.050000001"/>
					<Animation name="pistol_fire"/>
				</AnimLayer>
			</Fragment>
			<Fragment BlendOutDuration="0.2" Tags="AmericanShotgun">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.050000001"/>
					<Animation name="shotgun_fire"/>
				</AnimLayer>
			</Fragment>
		</shoot>
		<lower>
			<Fragment BlendOutDuration="0.2" Tags="Pistol">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.050000001"/>
					<Animation name="pistol_lower"/>
				</AnimLayer>
			</Fragment>
			<Fragment BlendOutDuration="0.2" Tags="AmericanShotgun">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.050000001"/>
					<Animation name="shotgun_lower"/>
				</AnimLayer>
			</Fragment>
		</lower>
		<raise>
			<Fragment BlendOutDuration="0.2" Tags="Pistol">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.050000001"/>
					<Animation name="pistol_raise"/>
				</AnimLayer>
			</Fragment>
			<Fragment BlendOutDuration="0.2" Tags="AmericanShotgun">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.050000001"/>
					<Animation name="shotgun_raise"/>
				</AnimLayer>
			</Fragment>
		</raise>
	</FragmentList>
</AnimDB>