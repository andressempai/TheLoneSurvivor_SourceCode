<AnimDB FragDef="animations/mannequin/adb/zombie_fragmentIds.xml" TagDef="animations/mannequin/adb/zombie_tags.xml">
	<FragmentList>
		<idle>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="perderos_idle" flags="Loop"/>
				</AnimLayer>
			</Fragment>
		</idle>
		<walk>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="perderos_boost_fwd" flags="Loop"/>
				</AnimLayer>
			</Fragment>
		</walk>
		<melee>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.1"/>
					<Animation name="perderos_attack_right_arm"/>
				</AnimLayer>
			</Fragment>
		</melee>
		<hit_reaction_fwd>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.1"/>
					<Animation name="perderos_hit_reaction_fwd"/>
				</AnimLayer>
			</Fragment>
		</hit_reaction_fwd>
		<dead>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.1"/>
					<Animation name="perderos_death"/>
				</AnimLayer>
			</Fragment>
		</dead>
	</FragmentList>
</AnimDB>